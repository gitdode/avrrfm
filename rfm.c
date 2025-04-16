/*
 * File:   rfm.c
 * Author: torsten.roemer@luniks.net
 *
 * Created on 24.03.2025, 21:07
 */

#include <util/delay.h>

#include "librfm95.h"
#include "pins.h"
#include "spi.h"

void _rfmDelay5() {
    _delay_ms(5);
}

void _rfmOn() {
#if RFM == 69
    PORT_RFM &= ~(1 << PIN_RRST);
#endif
#if RFM == 95
    PORT_RFM |= (1 << PIN_RRST);
#endif
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