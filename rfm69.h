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

#define FIFO      0x00
#define OP_MODE   0x01

#define NODE_ADDR 0x39

/**
 * Initializes the radio module.
 */
void initRadio(void);

/**
 * Do super useful stuff.
 */
void doStuff(void);

#endif /* RFM69_H */

