/* 
 * File:   spi.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 1. April 2023, 19:12
 */

#ifndef SPI_H
#define SPI_H

#include <stdio.h>
#include <avr/io.h>
#include "pins.h"

/**
 * Sets slow SPI speed.
 */
void spiSlow(void);

/**
 * Sets medium SPI speed.
 */
void spiMid(void);

/**
 * Sets fast SPI speed.
 */
void spiFast(void);

/**
 * Transmits the given byte and returns the byte reveived at the same time.
 * @param data byte to be written
 * @return byte read while writing
 */
uint8_t transmit(uint8_t data);

#endif /* SPI_H */

