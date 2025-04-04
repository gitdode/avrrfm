/*
 * File:   rfm.c
 * Author: torsten.roemer@luniks.net
 *
 * Created on 24.03.2025, 21:07
 */

#include <util/delay.h>

#include "librfm.h"
#include "pins.h"
#include "spi.h"

void _rfmDelay5() {
    _delay_ms(5);
}

void _rfmOn() {
    PORT_RFM |= (1 << PIN_RRST);
}

void _rfmSel() {
    PORT_RFM &= ~(1 << PIN_RCS);
}

void _rfmDes() {
    PORT_RFM |= (1 << PIN_RCS);
}

uint8_t _rfmTx(uint8_t data) {
    return transmit(data);
}