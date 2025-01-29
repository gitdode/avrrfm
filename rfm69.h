/* 
 * File:   rfm69.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 28. Januar 2025, 19:55
 */

#ifndef RFM69_H
#define RFM69_H

#include <util/delay.h>

#include "rfm69.h"
#include "pins.h"
#include "spi.h"
#include "usart.h"

#define FIFO        0x00
#define OP_MODE     0x01

#define FRF_MSB     0x07
#define FRF_MID     0x08
#define FRF_LSB     0x09

#define NODE_ADDR   0x39

/**
 * Initializes the radio module with the given carrier frequency in kilohertz.
 */
void initRadio(uint32_t kHz);

/**
 * Sets the carrier frequency in kilohertz.
 * 
 * @param kHz
 */
void setFreq(uint32_t kHz);

/**
 * Do super useful stuff.
 */
void doStuff(void);

#endif /* RFM69_H */

