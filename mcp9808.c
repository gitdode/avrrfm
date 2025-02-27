/* 
 * File:   mcp9808.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 9. Februar 2025, 20:41
 */

#include <util/delay.h>

#include "mcp9808.h"
#include "usart.h"

/**
 * Writes the given value to the given register,
 * MSB first.
 * 
 * @param address
 * @param value
 */
static void regWrite(uint8_t address, uint16_t value) {
    i2cStart();
    i2cSend(MCP9808_WRITE);
    i2cSend(address);
    i2cSend(value >> 8);
    i2cSend(value & 0x00ff);
    i2cStop();
}

/**
 * Reads and returns the value from the given register,
 * MSB first.
 * 
 * @param address
 * @return register value
 */
static uint16_t regRead(uint8_t address) {
    i2cStart();
    i2cSend(MCP9808_WRITE);
    i2cSend(address);
    i2cStart();
    i2cSend(MCP9808_READ);
    uint8_t upper = i2cReadAck();
    uint8_t lower = i2cReadNack();
    i2cStop();

    uint16_t value = 0;
    value |= upper << 8;
    value |= lower;

    return value;
}

void sleepTSens(void) {
    uint16_t conf = regRead(MCP9808_CONF);
    conf |= (1 << 8);
    regWrite(MCP9808_CONF, conf);
}

void wakeTSens(void) {
    uint16_t conf = regRead(MCP9808_CONF);
    conf &= ~(1 << 8);
    regWrite(MCP9808_CONF, conf);

    // TODO need to wait for first conversion result after wake up
    // can use an alert irq to wake up sleeping MCU instead?
    _delay_ms(300);
}

uint16_t readTSens(void) {
    uint16_t raw = regRead(MCP9808_TEMP);

    return raw;
}

int16_t convertTSens(uint16_t raw) {
    uint8_t upper = raw >> 8;
    uint8_t lower = raw & 0x00ff;

    // check flags
    if ((upper & 0x80) == 0x80) {
        // T >= TA CRIT
    }
    if ((upper & 0x40) == 0x40) {
        // T > TA UPPER
    }
    if ((upper & 0x20) == 0x20) {
        // T < TA LOWER
    }

    // clear flag bits
    upper &= 0x1F;
    int16_t temp;
    if ((upper & 0x10) == 0x10) {
        // TA < 0°C
        // clear sign
        upper &= 0x0f;
        temp = 2560 - (upper * 160 + lower * 10 / 16);
    } else {
        // TA >= 0°C
        temp = (upper * 160 + lower * 10 / 16);
    }

    return temp;
}