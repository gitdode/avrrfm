/* 
 * File:   i2c.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 15. Dezember 2023, 19:01
 */

#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <util/twi.h> // TODO check status register

#include "pins.h"

/**
 * Begins a message.
 */
void i2cStart(void);

/**
 * Ends a message.
 */
void i2cStop(void);

/**
 * Transmits the given byte of data.
 * 
 * @param data
 */
void i2cSend(uint8_t data);

/**
 * Reads one byte with ack.
 * 
 * @return data
 */
uint8_t i2cReadAck(void);

/**
 * Reads one byte without ack.
 * 
 * @return 
 */
uint8_t i2cReadNack(void);

#endif /* I2C_H */

