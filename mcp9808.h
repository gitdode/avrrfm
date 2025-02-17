/* 
 * File:   mcp9808.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 9. Februar 2025, 20:41
 */

#ifndef MCP9808_H
#define MCP9808_H

#include "i2c.h"

#define MCP9808_WRITE   (0x18 << 1) & 0xfe
#define MCP9808_READ    (0x18 << 1) | 0x01
#define MCP9808_CONF    0x01
#define MCP9808_TEMP    0x05

/**
 * Shuts down the temperature sensor.
 */
void sleepTSens(void);

/**
 * Wakes up the temperature sensor.
 */
void wakeTSens(void);

/**
 * Reads the current raw temperature and returns it.
 * 
 * @return temperature
 */
uint16_t readTSens(void);

/**
 * Converts the given raw temperature and returns it in °C x 10.
 * 
 * @return temperature x 10
 */
int16_t convertTSens(uint16_t raw);

#endif /* MCP9808_H */

