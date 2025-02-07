/* 
 * File:   i2c.c
 * Author: torsten.roemer@luniks.net
 *
 * Created on 15. Dezember 2023, 19:01
 */

#include "i2c.h"

void i2cStart(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
    loop_until_bit_is_set(TWCR, TWINT);
}

void i2cStop(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void i2cSend(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    loop_until_bit_is_set(TWCR, TWINT);
}

uint8_t i2cReadAck(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    loop_until_bit_is_set(TWCR, TWINT);

    return TWDR;
}

uint8_t i2cReadNack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    loop_until_bit_is_set(TWCR, TWINT);

    return TWDR;
}