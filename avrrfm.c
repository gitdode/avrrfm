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
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "pins.h"
#include "usart.h"
#include "spi.h"
#include "utils.h"
#include "rfm69.h"

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
    DDR_I2C |= (1 << PIN_SCL);
    DDR_I2C |= (1 << PIN_SDA);

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
    // 100 kHz @ 16 MHz
    TWBR = 72;
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

int main(void) {
    initUSART();
    initPins();
    initSPI();
    initI2C();
    initRadioInt();

    // enable global interrupts
    sei();

    printString("Hello Radio!\r\n");

    initRadio(868600);
    
    bool tx = false;

    while (true) {
        if (tx) {
            // uint8_t payload[] = {123};
            // transmitPayload(payload, sizeof(payload));
            char hello[] = "Hello Radio!";
            transmitPayload((uint8_t*)hello, sizeof(hello) - 1);
            _delay_ms(1000);
        } else {
            uint8_t payload[FIFO_SIZE];
            memset(payload, 0, FIFO_SIZE);
            uint8_t len = receivePayload(payload, sizeof (payload));
            printPayload(payload, len);
        }
    }

    return 0;
}
