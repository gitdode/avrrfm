/*
 * File:   spi.c
 * Author: torsten.roemer@luniks.net
 *
 * Created on 1. April 2023, 18:59
 */

#include "spi.h"

void spiSlow(void) {
    // fOSC/64
    SPCR |= (1 << SPR0);
    SPCR |= (1 << SPR1);
    SPSR |= (1 << SPI2X);
}

void spiMid(void) {
    // fOSC/16
    SPCR &= ~(1 << SPR0);
    SPCR |= (1 << SPR1);
    SPSR &= ~(1 << SPI2X);
}

void spiFast(void) {
    // fOSC/4 (default)
    SPCR &= ~(1 << SPR1) & ~(1 << SPR0);
    SPSR &= ~(1 << SPI2X);
}

uint8_t transmit(uint8_t data) {
    SPDR = data;
    loop_until_bit_is_set(SPSR, SPIF);

    return SPDR;
}
