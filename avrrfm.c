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

#define MEASURE_INTS 1

/* 1 int = 8 seconds */
static volatile uint8_t ints = 0;

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

    // drive output pins high
    PORT_RFM |= (1 << PIN_RCS);
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
 * Reads the temperature from the senor and transmits it.
 */
static void transmitTemp(void) {
    uint16_t temp = readTemp();
    uint8_t payload[] = {(temp >> 8), temp & 0x00ff};
    transmitPayload(payload, sizeof (payload));
    // printString("Transmitted\r\n");
}

/**
 * Receives the temperature and converts it to °C.
 */
static void receiveTemp(void) {
    printString("Receiving... ");
    uint8_t payload[2];
    receivePayload(payload, sizeof (payload));
    uint16_t raw = 0;
    raw |= payload[0] << 8;
    raw |= payload[1];

    int16_t tempx10 = convertTemp(raw);
    div_t temp = div(tempx10, 10);
    static char buf[16];
    snprintf(buf, sizeof (buf), "%d.%d°C\r\n", temp.quot, abs(temp.rem));
    printString(buf);
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

    bool tx = true;

    while (true) {
        if (tx) {
            if (ints % MEASURE_INTS == 0) {
                ints = 0;

                enableSPI();
                wakeTemp();
                wakeRadio();
                transmitTemp();
                sleepTemp();
                sleepRadio();
                disableSPI();
            }

            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            sleep_mode();
        } else {
            receiveTemp();
        }
    }

    return 0;
}
