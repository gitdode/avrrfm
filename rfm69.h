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

/**
 * Initializes the radio module.
 */
void initRadio(void);

#endif /* RFM69_H */

