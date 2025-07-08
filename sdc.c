/*
 * File:   sdc.c
 * Author: torsten.roemer@luniks.net
 *
 * Created on 25.03.2025, 21:13
 */

#include <util/delay.h>

#include "libsdc/libsdc.h"
#include "pins.h"
#include "spi.h"

void _sdcDelay10() {
    _delay_ms(10);
}

void _sdcSel() {
    PORT_SDC &= ~(1 << PIN_SDCS);
}

void _sdcDes() {
    PORT_SDC |= (1 << PIN_SDCS);
}

uint8_t _sdcTx(uint8_t data) {
    return transmit(data);
}