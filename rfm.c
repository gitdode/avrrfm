/*
 * File:   rfm.c
 * Author: torsten.roemer@luniks.net
 *
 * Created on 24.03.2025, 21:07
 */

#include <util/delay.h>

#if RFM == 69
    #include "librfm69/librfm69.h"
#endif
#if RFM == 95
    #include "librfm95/librfm95.h"
#endif
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