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

#include "pins.h"
#include "usart.h"
#include "spi.h"
#include "utils.h"
#include "librfm.h"
#include "mcp9808.h"
#include "sdcard.h"
#include "tft.h"
#include "display.h"
#include "dejavu.h"
#include "unifont.h"

#define TRANSMIT_FAST   4  // 4 ~ 32 seconds
#define TRANSMIT_SLOW   38 // 38 ~ 5 minutes

#define LABEL_OFFSET    10
#define BLACK           0x0000
#define RED             0xf800
#define WHITE           0xffff

#define NODE0   0x12
#define NODE1   0x24
#define NODE2   0x42

#ifndef RECEIVER
    #define RECEIVER    0
#endif

static volatile uint8_t watchdogInts = 0;
static uint8_t measureInts = TRANSMIT_FAST;
static uint8_t timeoutCount = 0;

/* Temp. label coordinates */
static x_t xl = 0;
static y_t yl = 0;
static x_t xo = 0;
static y_t yo = 0;
static width_t width = 0;

/**
 * Called when the watchdog barks to wake up the transmitter.
 */
ISR(WDT_vect) {
    watchdogInts++;
}

/**
 * Called while the controller isn't in power down sleep mode.
 */
ISR(TIMER0_COMPA_vect) {
    rfmTimer();
}

/**
 * Called when an INT0 interrupt occurs.
 */
ISR(INT0_vect) {
    rfmInt();
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
 * Enables radio interrupt.
 */
static void initRadioInt(void) {
    EIMSK |= (1 << INT0);
    // EICRA |= (1 << ISC00); // interrupt on any logical change
    // EICRA |= (1 << ISC01); // interrupt on falling edge
    EICRA |= (1 << ISC01) | (1 << ISC00); // interrupt on rising edge
}

/**
 * Sets up the watchdog.
 */
static void initWatchdog(void) {
    cli();
    wdt_reset();
    // watchdog change enable
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    // enable interrupt, disable system reset, bark every 8 seconds
    WDTCSR = (1 << WDIE) | (0 << WDE) | (1 << WDP3) | (1 << WDP0);
}

/**
 * Sets up the timer.
 */
static void initTimer(void) {
    // timer0 clear timer on compare match mode, TOP OCR0A
    TCCR0A |= (1 << WGM01);
    // timer0 clock prescaler/1024/255 ~ 46 Hz @ 12 MHz ~ 61 Hz @ 16 MHz
    TCCR0B |= (1 << CS02) | (1 << CS00);
    OCR0A = 255;

    // enable timer0 compare match A interrupt
    TIMSK0 |= (1 << OCIE0A);
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
 * Reads the temperature from the sensor and transmits it with the given
 * node address.
 */
static void transmitTemp(uint8_t node) {
    uint16_t temp = readTSens();
    uint8_t power = rfmGetOutputPower();
    uint8_t payload[] = {(temp >> 8), temp & 0x00ff, power};
    rfmTransmitPayload(payload, sizeof (payload), node);
    // printString("Transmitted\r\n");
}

/**
 * Converts the raw temperature to °C and lets it float around the display.
 *
 * @param rssi RSSI value
 * @param crc CRC result
 * @param temp temperature + info
 */
static void displayTemp(uint8_t rssi, bool crc, Temperature *temp) {
    uint8_t _rssi = divRoundNearest(rssi, 2);
    int16_t tempx10 = convertTSens(temp->raw);
    div_t tdiv = div(tempx10, 10);

    char buf[42];

    // display some info (receiver RSSI + CRC, transmitter output power)
    snprintf(buf, sizeof (buf), "RSSI: %4d dBm, CRC: %d, PA: %+3d dBm",
            -_rssi, crc, -18 + (temp->power & 0x1f));
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
 * Reads and returns the raw temperature + other info.
 *
 * @return temperature + info
 */
static Temperature readTemp(void) {
    uint8_t payload[3];
    rfmReadPayload(payload, sizeof (payload));
    Temperature temp = {0};
    temp.raw |= payload[0] << 8;
    temp.raw |= payload[1];
    temp.power = payload[2];

    return temp;
}

/**
 * Handles the payload received from the transmitter.
 *
 * @param flags
 */
static void handlePayload(PayloadFlags flags) {
    uint8_t rssi = rfmGetRssi();
    Temperature temp = readTemp();

    // communicate RSSI back to transmitter
    uint8_t payload[] = {rssi};
    rfmTransmitPayload(payload, sizeof (payload), NODE2);

    displayTemp(rssi, flags.crc, &temp);
    rfmStartReceive();
}

/**
 * Waits for a response from the receiver with timeout.
 *
 * @return true if timeout occurred, false otherwise
 */
static bool waitResponse(void) {
    uint8_t response[1];
    int8_t len = rfmReceivePayload(response, sizeof (response), true);
    if (len > 0) {
        // receiver RSSI
        int8_t rssi = divRoundNearest(response[0], 2);
        rfmSetOutputPower(rssi);

        return false;
    }

    return true;
}

/**
 * Receives data read from SD card.
 *
 * @param flags
 */
static void receiveData(PayloadFlags flags) {
    uint8_t payload[MESSAGE_SIZE + 1]; // + address byte
    uint8_t len = rfmReadPayload(payload, sizeof (payload));

    char buf[MESSAGE_SIZE + 1];
    snprintf(buf, len, "%s", payload);
    printString(buf);
    _delay_ms(10);

    rfmStartReceive();
}

/**
 * Transmits data read from SD card.
 */
static void transmitData(void) {
    uint8_t block[SD_BLOCK_SIZE];
    bool read = readSingleBlock(0, block);
    if (read) {
        void *start = &block;
        div_t packets = div(SD_BLOCK_SIZE, MESSAGE_SIZE);
        for (size_t i = 0; i < packets.quot; i++) {
            rfmTransmitPayload(start, MESSAGE_SIZE, NODE0);
            start += MESSAGE_SIZE;
            // a little break in between packets for now
            _delay_ms(100);
        }
        if (packets.rem > 0) {
            rfmTransmitPayload(start, packets.rem, NODE0);
        }
    }
}

int main(void) {
    initUSART();
    initPins();
    initSPI();
    initI2C();
    initRadioInt();
    bool sdcard = false;

    printString("Hello Radio!\r\n");

    if (!RECEIVER) {
        // used only for tx
        initWatchdog();
        initTimer();
        sdcard = initSDCard();
    }

    // enable global interrupts
    sei();

    uint8_t node = RECEIVER ? NODE1 : NODE2;
    rfmInit(868600, node);
    if (RECEIVER) {
        initDisplay();
        setFrame(WHITE);
        fillArea(0, 0, DISPLAY_WIDTH, 16, BLACK);
        // initial rx mode
        rfmStartReceive();
    }

    while (true) {
        // do something else besides tx/rx
        // printString("Running...\r\n");
        // _delay_ms(1000);

        if (!RECEIVER) {
            if (watchdogInts % measureInts == 0) {
                watchdogInts = 0;

                enableSPI();
                wakeTSens();
                rfmWake();
                if (sdcard) {
                    transmitData();
                } else {
                    transmitTemp(NODE1);
                    bool timeout = waitResponse();
                    if (timeout) {
                        if (++timeoutCount > MAX_TIMEOUTS) {
                            measureInts = TRANSMIT_SLOW;
                            timeoutCount = 0;
                        }
                    } else {
                        timeoutCount = 0;
                        measureInts = TRANSMIT_FAST;
                    }
                }
                sleepTSens();
                rfmSleep();
                disableSPI();
            }
        } else {
            PayloadFlags flags = rfmPayloadReady();
            if (flags.ready) {
                handlePayload(flags);
                // receiveData(flags);
            }
        }

        // power down until woken up by watchdog (tx)
        // or "PayloadReady" (rx) interrupt
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();
    }

    return 0;
}
