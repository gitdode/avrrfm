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

#define MEASURE_INTS    4
#define LABEL_OFFSET    10

#define BLACK   0x0000
#define RED     0xf800
#define WHITE   0xffff

#ifndef RECEIVER
    #define RECEIVER    1
#endif

/* 1 int = 8 seconds */
static volatile uint8_t ints = 0;

/* Temp. label coordinates */
static x_t xl = 0;
static y_t yl = 0;
static x_t xo = 0;
static y_t yo = 0;
static width_t width = 0;

ISR(WDT_vect) {
    ints++;
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
 * Reads the temperature from the sensor and transmits it.
 */
static void transmitTemp(void) {
    uint16_t temp = readTSens();
    uint8_t payload[] = {(temp >> 8), temp & 0x00ff};
    transmitPayload(payload, sizeof (payload));
    // printString("Transmitted\r\n");
}

/**
 * Converts the raw temperature to °C and lets it float around the display.
 * 
 * @param raw temperature
 */
static void displayTemp(uint8_t rssi, bool crc, uint16_t raw) {
    uint8_t _rssi = divRoundNearest(rssi, 2);
    int16_t tempx10 = convertTSens(raw);
    div_t temp = div(tempx10, 10);
    
    static char buf[42];

    snprintf(buf, sizeof (buf), "RSSI: -%d dBm, CRC: %d, %d.%d°C\r\n", 
            _rssi, crc, temp.quot, abs(temp.rem));
    printString(buf);
    
    snprintf(buf, sizeof (buf), "RSSI: -%d dBm, CRC: %d", _rssi, crc);
    const __flash Font *unifont = &unifontFont;
    writeString(0, 0, unifont, buf, WHITE, BLACK);

    snprintf(buf, sizeof (buf), "%4d.%d°", temp.quot, abs(temp.rem));
    const __flash Font *dejaVu = &dejaVuFont;
    if (width > 0) fillArea(xo, yo, width, dejaVu->height, WHITE);
    if (yl == 0) yl = unifont->height;
    width = writeString(xl, yl, dejaVu, buf, WHITE, crc ? BLACK : RED);
    xo = xl;
    yo = yl;
    xl += LABEL_OFFSET;
    yl += LABEL_OFFSET;
    if (xl > DISPLAY_WIDTH - width) xl = 0;
    if (yl > DISPLAY_HEIGHT - dejaVu->height) yl = unifont->height;
}

/**
 * Reads and returns the raw temperature.
 * 
 * @return raw temp
 */
static uint16_t readTemp(void) {
    uint8_t payload[2];
    readPayload(payload, sizeof (payload));
    uint16_t raw = 0;
    raw |= payload[0] << 8;
    raw |= payload[1];

    return raw;
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
    }

    // enable global interrupts
    sei();

    printString("Hello Radio!\r\n");

    initRadio(868600);
    if (RECEIVER) {
        initDisplay();

        setFrame(0xffff);
    
        // initial rx mode
        startReceive();
    }

    while (true) {
        // do something else besides tx/rx
        // printString("Running...\r\n");
        // _delay_ms(1000);

        if (!RECEIVER) {
            if (ints % MEASURE_INTS == 0) {
                ints = 0;

                enableSPI();
                wakeTSens();
                wakeRadio();
                transmitTemp();
                sleepTSens();
                sleepRadio();
                disableSPI();
            }
        } else {
            PayloadFlags flags = payloadReady();
            if (flags.ready) {
                uint8_t rssi = readRssi();
                uint16_t raw = readTemp();
                displayTemp(rssi, flags.crc, raw);
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
