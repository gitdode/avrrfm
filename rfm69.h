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
#include "types.h"
#include "spi.h"
#include "usart.h"

#define FIFO        0x00
#define OP_MODE     0x01
#define DATA_MOD    0x02
#define BITRATE_MSB 0x03
#define BITRATE_LSB 0x04
#define FDEV_MSB    0x05
#define FDEV_LSB    0x06
#define FRF_MSB     0x07
#define FRF_MID     0x08
#define FRF_LSB     0x09
#define OSC1        0x0a
#define PA_LEVEL    0x11
#define LNA         0x18
#define RX_BW       0x19
#define AFC_FEI     0x1e
#define AFC_BW      0x20
#define RSSI_CONFIG 0x23
#define RSSI_VALUE  0x24
#define DIO_MAP1    0x25
#define DIO_MAP2    0x26
#define IRQ_FLAGS1  0x27
#define RSSI_THRESH 0x29
#define RX_TIMEOUT1 0x2a
#define RX_TIMEOUT2 0x2b
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
#define AUTO_MODES  0x3b
#define FIFO_THRESH 0x3c
#define PCK_CFG2    0x3d
#define TEST_LNA    0x58
#define TEST_PA1    0x5a
#define TEST_PA2    0x5c
#define TEST_DAGC   0x6f
#define TEST_AFC    0x71

#define MASK_MODE   0x1c

#define MODE_SLEEP  0x00
#define MODE_STDBY  0x04
#define MODE_FS     0x08
#define MODE_TX     0x0c
#define MODE_RX     0x10

#define FIFO_SIZE       64
#define F_STEP          6103515625ULL
#define CAST_ADDRESS    0x84

/**
 * Initializes the radio module with the given carrier frequency in kilohertz
 * and node address.
 */
void initRadio(uint64_t freq, uint8_t node);

/**
 * Indicates a timeout.
 */
void timeoutRadio(void);

/**
 * Shuts down the radio.
 */
void sleepRadio(void);

/**
 * Wakes up the radio.
 */
void wakeRadio(void);

/**
 * Sets the radio to receive mode and maps "PayloadReady" to DIO0.
 */
void startReceive(void);

/**
 * Returns the current RSSI value.
 * 
 * @return rssi value
 */
uint8_t readRssi(void);

/**
 * Returns true if a "PayloadReady" interrupt arrived and clears the
 * interrupt state.
 * 
 * @return true if "PayloadReady"
 */
PayloadFlags payloadReady(void);

/**
 * Sets the radio in standby mode, puts the payload into the given array 
 * with the given size, and returns the length of the payload.
 * 
 * @param payload buffer for payload
 * @param size of payload buffer
 * @return payload bytes actually received
 */
size_t readPayload(uint8_t *payload, size_t size);

/**
 * Waits for "PayloadReady", puts the payload into the given array with the 
 * given size, and returns the length of the payload, or 0 if a timeout 
 * occurred.
 * 
 * @param payload buffer for payload
 * @param size of payload buffer
 * @return payload bytes actually received
 */
size_t receivePayload(uint8_t *payload, size_t size);

/**
 * Transmits up to 64 bytes of the given payload with the given node address.
 * 
 * @param payload to be sent
 * @param size of payload
 * @param node address
 * @return payload bytes actually sent
 */
size_t transmitPayload(uint8_t *payload, size_t size, uint8_t node);

#endif /* RFM69_H */

