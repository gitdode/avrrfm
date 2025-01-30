/* 
 * File:   rfm69.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 28. Januar 2025, 19:55
 */

#ifndef RFM69_H
#define RFM69_H

#include <stdbool.h>
#include <math.h>
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
#define DIO_MAP1    0x25
#define DIO_MAP2    0x26
#define IRQ_FLAGS1  0x27
#define PREAMB_MSB  0x2c
#define PREAMB_LSB  0x2d
#define IRQ_FLAGS2  0x28
#define PCK_CFG1    0x37
#define NODE_ADDR   0x39
#define FIFO_THRESH 0x3c

#define MASK_MODE   0x0e

#define MODE_SLEEP  0x00
#define MODE_STDBY  0x04
#define MODE_FS     0x08
#define MODE_TX     0x0c
#define MODE_RX     0x10

/**
 * Initializes the radio module with the given carrier frequency in kilohertz.
 */
void initRadio(uint32_t kHz);

/**
 * Sets the carrier frequency in kilohertz.
 * 
 * @param kHz
 */
void setFreq(uint32_t freq);

/**
 * Transmits one byte and prints some info.
 */
void sendByte(uint8_t payload);

/**
 * Transmits up to 64 characters of the given string, excluding the
 * terminal '\\0' character. 
 * 
 * @param payload string to be sent
 * @return characters sent
 */
size_t sendString(char *payload);

#endif /* RFM69_H */

