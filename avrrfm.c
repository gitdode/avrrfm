/*
 * avrrfm.c
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2.
 *
 * Experimental project to drive an RFM69HCW radio.
 *
 * Created on: 27.01.2025
 *     Author: torsten.roemer@luniks.net
 *
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "types.h"
#include "pins.h"
#include "usart.h"
#include "spi.h"
#include "utils.h"
#include "librfm.h"
#include "libsdc.h"
#include "mcp9808.h"
#include "tft.h"
#include "display.h"
#include "dejavu.h"
#include "unifont.h"

#define TRANSMIT_FAST   15  // 15 ~ 30 seconds
#define TRANSMIT_SLOW   150 // 150 ~ 5 minutes
#define MAX_TIMEOUTS    9   // slow down tx attempts after so many timeouts

#define LABEL_OFFSET    10
#define BLACK           0x0000
#define RED             0xf800
#define WHITE           0xffff

/* Node addresses */
#define NODE0           0x12
#define NODE1           0x24
#define NODE2           0x42

/* Carrier frequency in kHz */
#define FREQ            868600

/* Limit to FSK max size for now */
#define MSG_SIZE        RFM_FSK_MSG_SIZE

#ifndef RECEIVER
    #define RECEIVER    1
#endif

#define LORA            1

static volatile uint8_t watchdogInts = 0;
static uint8_t measureInts = TRANSMIT_FAST;
static uint8_t timeoutCount = 0;

/* Temp. label coordinates */
static x_t xl = 0;
static y_t yl = 0;
static x_t xo = 0;
static y_t yo = 0;
static width_t width = 0;

/* Averaged output power requested from transmitter */
static int8_t power = RFM_DBM_MAX;

/**
 * Wakes up the controller and increments the watchdog bark counter.
 */
ISR(WDT_vect) {
    watchdogInts++;
    rfmTimeout();
}

/**
 * Wakes up the controller and notifies of an interrupt on DIO.
 */
ISR(INT0_vect) {
    rfmIrq();
}

/**
 * Wakes up the controller and notifies of an interrupt on DIO4 (FSK)/
 * DIO1 (LoRa).
 */
ISR(INT1_vect) {
    rfmIrq();
}

/**
 * Sets up the pins.
 */
static void initPins(void) {
    // set MOSI and SCK as output pin
    DDR_SPI |= (1 << PIN_MOSI);
    DDR_SPI |= (1 << PIN_SCK);
    // drive SS (ensure master) and MISO high
    PORT_SPI |= (1 << PIN_SS);
    PORT_SPI |= (1 << PIN_MISO);

    // set SDA and SCL as output pin
    // DDR_I2C |= (1 << PIN_SCL);
    // DDR_I2C |= (1 << PIN_SDA);

    // set radio CS and RST pin as output pin
    DDR_RFM |= (1 << PIN_RCS);
    DDR_RFM |= (1 << PIN_RRST);

    // set SD card CS as output pin
    DDR_SDC |= (1 << PIN_SDCS);

    // set display CS, D/C and RST pin as output pin
    DDR_DISP |= (1 << PIN_DCS);
    DDR_DISP |= (1 << PIN_DDC);
    DDR_DISP |= (1 << PIN_DRST);

    // drive output pins high
    PORT_RFM |= (1 << PIN_RCS);
    PORT_SDC |= (1 << PIN_SDCS);
    PORT_DISP |= (1 << PIN_DCS);
    PORT_DISP |= (1 << PIN_DDC);
    PORT_DISP |= (1 << PIN_DRST);
}

/**
 * Enables SPI master mode.
 */
static void initSPI(void) {
    // default fOSC/4
    SPCR |= (1 << MSTR);
    SPCR |= (1 << SPE);
    if (!RECEIVER) {
        // slow down for the breadboard wiring
        spiMid();
    }
}

/**
 * Enables I2C.
 */
static void initI2C(void) {
    // 100 kHz @ 8 MHz
    TWBR = 32;
    TWCR |= (1 << TWEN);
}

/**
 * Enables radio interrupts.
 */
static void initRadioInt(void) {
    EIMSK |= (1 << INT0);
    // interrupt on rising edge
    EICRA |= (1 << ISC01) | (1 << ISC00);

    EIMSK |= (1 << INT1);
    // interrupt on rising edge
    EICRA |= (1 << ISC11) | (1 << ISC10);
}

/**
 * Sets up the watchdog.
 */
static void initWatchdog(void) {
    cli();
    wdt_reset();
    // watchdog change enable
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    // enable interrupt, disable system reset, bark every 2 seconds
    WDTCSR = (1 << WDIE) | (0 << WDE) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0);
}

/**
 * Enables SPI.
 */
static void enableSPI(void) {
    SPCR |= (1 << SPE);
}

/**
 * Disables SPI.
 */
static void disableSPI(void) {
    SPCR &= ~(1 << SPE);
    PORT_SPI &= ~(1 << PIN_SCK);
}

/**
 * Converts the raw temperature to °C and lets it float around the display.
 *
 * @param rssi RSSI value
 * @param crc CRC result
 * @param temp temperature + info
 */
static void displayTemp(uint8_t rssi, bool crc, Temperature *temp) {
    int16_t tempx10 = convertTSens(temp->raw);
    div_t tdiv = div(tempx10, 10);

    // display some info (receiver RSSI + CRC, transmitter output power)
    char paf[5];
    char buf[42];
    snprintf(paf, sizeof (paf), "%+3d", temp->power);
    snprintf(buf, sizeof (buf), "RSSI: %4d dBm, CRC: %d, PA: %s dBm",
            -rssi, crc, crc ? paf : "---");
    const __flash Font *unifont = &unifontFont;
    writeString(0, 0, unifont, buf, BLACK, WHITE);

    // display temperature (floating, red if CRC failed)
    snprintf(buf, sizeof (buf), "%c%d.%d°", tempx10 < 0 ? '-' : ' ',
            abs(tdiv.quot), abs(tdiv.rem));
    const __flash Font *dejaVu = &dejaVuFont;
    if (width > 0) fillArea(xo, yo, width, dejaVu->height, WHITE);
    if (yl == 0) yl = unifont->height;
    width = writeString(xl, yl, dejaVu, buf, WHITE, crc ? BLACK : RED);
    xo = xl;
    yo = yl;
    xl += LABEL_OFFSET;
    yl += LABEL_OFFSET;
    if (xl > DISPLAY_WIDTH - width - LABEL_OFFSET) xl = 0;
    if (yl > DISPLAY_HEIGHT - dejaVu->height- LABEL_OFFSET) yl = unifont->height;
}

/**
 * Reads the temperature from the given payload and returns it.
 *
 * @param payload
 * @return raw temperature + tx power in dBm
 */
static Temperature readTemp(uint8_t *payload) {
    Temperature temp = {0};
    temp.raw |= payload[0] << 8;
    temp.raw |= payload[1];
    temp.power = payload[2];

    return temp;
}

/**
 * Receives the temperature and responds with the RSSI.
 */
static void receiveTemp(void) {
    if (LORA) {
        RxFlags flags = rfmLoRaRxDone();
        if (flags.ready) {
            uint8_t response[] = {flags.rssi};
            rfmLoRaTx(response, sizeof (response));

            uint8_t payload[3];
            rfmLoRaRxRead(payload, sizeof (payload));
            Temperature temp = readTemp(payload);

            displayTemp(flags.rssi, flags.crc, &temp);

            rfmLoRaStartRx();
        }
    } else {
        RxFlags flags = rfmPayloadReady();
        if (flags.ready) {
            uint8_t response[] = {flags.rssi};
            rfmTransmitPayload(response, sizeof (response), NODE2);

            uint8_t payload[3];
            rfmReadPayload(payload, sizeof (payload));
            Temperature temp = readTemp(payload);
            displayTemp(flags.rssi, flags.crc, &temp);

            rfmStartReceive(false);
        }
    }
}

/**
 * Reads the temperature from the sensor and transmits it with the given
 * node address.
 *
 * @param node node address
 */
static void transmitTemp(uint8_t node) {
    uint16_t temp = readTSens();
    uint8_t power = rfmGetOutputPower();
    uint8_t payload[] = {(temp >> 8), temp & 0x00ff, power};
    if (LORA) {
        rfmLoRaTx(payload, sizeof (payload));
    } else {
        rfmTransmitPayload(payload, sizeof (payload), node);
    }
}

/**
 * Waits for a response from the receiver with timeout.
 *
 * @return true if timeout occurred, false otherwise
 */
static bool waitResponse(void) {
    uint8_t response[1];
    int8_t len = 0;
    if (LORA) {
        len = rfmLoRaRx(response, sizeof (response));
    } else {
        len = rfmReceivePayload(response, sizeof (response), true);
    }
    if (len > 0) {
        // set more output power starting from -100 dBm
        int8_t rssi = divRoundNearest(response[0], 2);
        power = divRoundNearest(power + rssi - 98, 2);
        rfmSetOutputPower(min(max(power, RFM_DBM_MIN), RFM_DBM_MAX));

        return false;
    }

    return true;
}

int main(void) {
    initUSART();
    initPins();
    initSPI();
    initI2C();
    initRadioInt();
    // bool sdcard = sdcInit();

    if (!RECEIVER) {
        // used only for tx
        initWatchdog();
    }

    // enable global interrupts
    sei();

    printString("Hello Radio!\r\n");

    uint8_t node = RECEIVER ? NODE1 : NODE2;
    bool radio = rfmInit(FREQ, node, LORA);
    if (!radio) {
        printString("Radio init failed!\r\n");
    }

    if (RECEIVER) {
        initDisplay();
        setFrame(WHITE);
        fillArea(0, 0, DISPLAY_WIDTH, 16, BLACK);
        // initial rx mode
        if (radio) {
            if (LORA) {
                rfmLoRaStartRx();
            } else {
                rfmStartReceive(false);
            }
        }
    }

    while (true) {
        // do something else besides tx/rx
        // printString("Running...\r\n");
        // _delay_ms(1000);

        if (radio) {
            if (!RECEIVER) {
                if (watchdogInts % measureInts == 0) {
                    watchdogInts = 0;

                    enableSPI();
                    wakeTSens();
                    rfmWake();
                    // transmit temperature
                    transmitTemp(NODE1);
                    // wait for response from receiver
                    bool timeout = waitResponse();
                    if (timeout) {
                        if (++timeoutCount > MAX_TIMEOUTS) {
                            measureInts = TRANSMIT_SLOW;
                            timeoutCount = 0;
                        }
                        rfmSetOutputPower(RFM_DBM_MAX);
                    } else {
                        timeoutCount = 0;
                        measureInts = TRANSMIT_FAST;
                    }
                    sleepTSens();
                    rfmSleep();
                    disableSPI();
                }
            } else {
                // receive and display temperature, respond back to transmitter
                receiveTemp();
            }
        }

        // power down until woken up by watchdog (tx)
        // or "PayloadReady" (rx) interrupt
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();
    }

    return 0;
}
