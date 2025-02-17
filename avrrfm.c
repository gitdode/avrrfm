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

#define MEASURE_INTS    8
#define LABEL_OFFSET    10

#ifndef RECEIVER
    #define RECEIVER    1
#endif

/* 1 int = 8 seconds */
static volatile uint8_t ints = 0;

/* Temp. label coordinates */
static x_t x = 0;
static y_t y = 0;
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
 * Prints the given payload with the given size.
 * 
 * @param payload
 * @param size
 */
static void printPayload(uint8_t *payload, size_t size) {
    printString("Length: ");
    printUint(size);
    printString("Payload:\r\n");
    char buf[FIFO_SIZE + 3];
    snprintf(buf, size + 3, "%s\r\n", payload);
    printString(buf);
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
static void displayTemp(uint16_t raw) {
    int16_t tempx10 = convertTSens(raw);
    div_t temp = div(tempx10, 10);
    static char buf[16];

    snprintf(buf, sizeof (buf), "%d.%d°C\r\n", temp.quot, abs(temp.rem));
    printString(buf);

    const __flash Font *dejaVu = &dejaVuFont;
    if (width > 0) fillArea(xo, yo, width, dejaVu->height, 0xffff);
    snprintf(buf, sizeof (buf), "%4d.%d°", temp.quot, abs(temp.rem));
    width = writeString(x, y, dejaVu, buf);
    xo = x;
    yo = y;
    x += LABEL_OFFSET;
    y += LABEL_OFFSET;
    if (x > DISPLAY_WIDTH - width) x = 0;
    if (y > DISPLAY_HEIGHT - dejaVu->height) y = 0;
}

/**
 * Blocks until the raw temperature is received, then returns it.
 * 
 * @return raw temp
 */
static uint16_t receiveTemp(void) {
    // printString("Receiving... ");
    uint8_t payload[2];
    receivePayload(payload, sizeof (payload));
    uint16_t raw = 0;
    raw |= payload[0] << 8;
    raw |= payload[1];

    return raw;
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
    initWatchdog();

    // enable global interrupts
    sei();

    printString("Hello Radio!\r\n");

    initRadio(868600);
    initDisplay();

    setFrame(0xffff);

    if (RECEIVER) {
        startReceive();
    }

    while (true) {
        if (! RECEIVER) {
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

            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            sleep_mode();
        } else {
            // uint16_t raw = receiveTemp();
            // displayTemp(raw);
            
            if (payloadReady()) {
                uint16_t raw = readTemp();
                displayTemp(raw);
                startReceive();
            }
            
            // do something else...
            printString("Running...\r\n");
            _delay_ms(1000);
            
            // and/or go to sleep until next watchdog bark or payload received
            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            sleep_mode();
        }
    }

    return 0;
}
