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
#include "rfm69.h"
#include "mcp9808.h"
#include "tft.h"
#include "display.h"
#include "dejavu.h"
#include "unifont.h"

#define MEASURE_INTS    4  // about 32 seconds
#define TIMEOUT_INTS    30 // about one second

#define LABEL_OFFSET    10

#define BLACK   0x0000
#define RED     0xf800
#define WHITE   0xffff

#define NODE1   0x24
#define NODE2   0x42

#ifndef RECEIVER
    #define RECEIVER    1
#endif

/* 1 int = 8 seconds */
static volatile uint8_t wdints = 0;

static volatile bool toena = false;
static volatile uint8_t toints = 0;

/* Temp. label coordinates */
static x_t xl = 0;
static y_t yl = 0;
static x_t xo = 0;
static y_t yo = 0;
static width_t width = 0;

ISR(WDT_vect) {
    wdints++;
}

ISR(TIMER0_COMPA_vect) {
    if (toena && toints++ >= TIMEOUT_INTS) {
        toints = 0;
        toena = false;
        timeoutRadio();
    }
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

    // set display CS, D/C and RST pin as output pin
    DDR_DISP |= (1 << PIN_DCS);
    DDR_DISP |= (1 << PIN_DDC);
    DDR_DISP |= (1 << PIN_DRST);

    // drive output pins high
    PORT_RFM |= (1 << PIN_RCS);
    PORT_DISP |= (1 << PIN_DCS);
    PORT_DISP |= (1 << PIN_DDC);
    PORT_DISP |= (1 << PIN_DRST);
}

/**
 * Enables SPI master mode.
 */
static void initSPI(void) {
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
    uint8_t power = getOutputPower();
    uint8_t payload[] = {(temp >> 8), temp & 0x00ff, power};
    transmitPayload(payload, sizeof (payload), node);
    // printString("Transmitted\r\n");
}

/**
 * Waits for a response from the receiver with timeout.
 */
static void waitResponse(void) {
    toena = true;
    uint8_t response[1];
    int8_t len = receivePayload(response, sizeof (response));
    if (len > 0) {
        // receiver RSSI
        int8_t rssi = divRoundNearest(response[0], 2);
        setOutputPower(rssi);
    }
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
    readPayload(payload, sizeof (payload));
    Temperature temp = {0};
    temp.raw |= payload[0] << 8;
    temp.raw |= payload[1];
    temp.power = payload[2];

    return temp;
}

int main(void) {
    initUSART();
    initPins();
    initSPI();
    initI2C();
    initRadioInt();
    if (!RECEIVER) {
        // used only for tx
        initWatchdog();
        initTimer();
    }

    // enable global interrupts
    sei();

    printString("Hello Radio!\r\n");

    uint8_t node = RECEIVER ? NODE1 : NODE2;
    initRadio(868600, node);
    if (RECEIVER) {
        initDisplay();
        setFrame(WHITE);
        fillArea(0, 0, DISPLAY_WIDTH, 16, BLACK);
        // initial rx mode
        startReceive();
    }

    while (true) {
        // do something else besides tx/rx
        // printString("Running...\r\n");
        // _delay_ms(1000);

        if (!RECEIVER) {
            if (wdints % MEASURE_INTS == 0) {
                wdints = 0;

                enableSPI();
                wakeTSens();
                wakeRadio();
                transmitTemp(NODE1);
                waitResponse();
                sleepTSens();
                sleepRadio();
                disableSPI();
            }
        } else {
            PayloadFlags flags = payloadReady();
            if (flags.ready) {
                uint8_t rssi = readRssi();
                Temperature temp = readTemp();

                // TODO delay?
                _delay_ms(10);
                uint8_t payload[] = {rssi};
                transmitPayload(payload, sizeof (payload), NODE2);

                displayTemp(rssi, flags.crc, &temp);
                startReceive();
            }
        }

        // power down until woken up by watchdog (tx)
        // or "PayloadReady" (rx) interrupt
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();
    }

    return 0;
}
