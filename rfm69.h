/* 
 * File:   rfm69.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 28. Januar 2025, 19:55
 */

#ifndef RFM69_H
#define RFM69_H

#include <stdbool.h>
#include <util/delay.h>

#include "rfm69.h"
#include "pins.h"
#include "spi.h"
#include "usart.h"

#define FIFO        0x00
#define OP_MODE     0x01
#define DATA_MOD    0x02
#define BITRATE_MSB 0x03
#define BITRATE_LSB 0x04
#define FRF_MSB     0x07
#define FRF_MID     0x08
#define FRF_LSB     0x09
#define OSC1        0x0a
#define PA_LEVEL    0x11
#define LNA         0x18
#define RX_BW       0x19
#define AFC_BW      0x20
#define DIO_MAP1    0x25
#define DIO_MAP2    0x26
#define IRQ_FLAGS1  0x27
#define PREAMB_MSB  0x2c
#define PREAMB_LSB  0x2d
#define IRQ_FLAGS2  0x28
#define SYNC_CONF   0x2e
#define SYNC_VAL1   0x2f
#define SYNC_VAL2   0x30
#define SYNC_VAL3   0x31
#define SYNC_VAL4   0x32
#define SYNC_VAL5   0x33
#define SYNC_VAL6   0x34
#define SYNC_VAL7   0x35
#define SYNC_VAL8   0x36
#define PCK_CFG1    0x37
#define NODE_ADDR   0x39
#define CAST_ADDR   0x3a
#define FIFO_THRESH 0x3c

#define MASK_MODE   0x1c

#define MODE_SLEEP  0x00
#define MODE_STDBY  0x04
#define MODE_FS     0x08
#define MODE_TX     0x0c
#define MODE_RX     0x10

#define FIFO_SIZE       64
#define NODE_ADDRESS    0x42
#define CAST_ADDRESS    0x84

/**
 * Initializes the radio module with the given carrier frequency in kilohertz.
 */
void initRadio(uint32_t kHz);

/**
 * Shuts down the radio.
 */
void sleepRadio(void);

/**
 * Wakes up the radio.
 */
void wakeRadio(void);

/**
 * Waits for a packet, puts the payload into the given array with the 
 * given size, and returns the length of the payload.
 * 
 * @param payload buffer for payload
 * @return bytes actually received
 */
size_t receivePayload(uint8_t *payload, size_t size);

/**
 * Transmits up to 64 bytes of the given payload.
 * 
 * @param payload to be sent
 * @return bytes actually sent
 */
size_t transmitPayload(uint8_t *payload, size_t size);

#endif /* RFM69_H */

