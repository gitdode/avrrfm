/*
 * File:   tft.c
 * Author: torsten.roemer@luniks.net
 *
 * Created on 06.07.2025, 20:26
 */

#include <util/delay.h>

#include "libtft/libtft.h"
#include "pins.h"
#include "spi.h"

void _tftDelay100() {
    _delay_ms(100);
}

void _tftSel() {
    PORT_DISP &= ~(1 << PIN_DCS);
}

void _tftDes() {
    PORT_DISP |= (1 << PIN_DCS);
}

void _tftSetCmd(void) {
    PORT_DISP &= ~(1 << PIN_DDC);
}

void _tftSetData(void){
    PORT_DISP |= (1 << PIN_DDC);
}

void _tftReset(void) {
    PORT_DISP &= ~(1 << PIN_DRST);
    _delay_ms(10);
    PORT_DISP |= (1 << PIN_DRST);
}

uint8_t _tftTx(uint8_t data) {
    return transmit(data);
}