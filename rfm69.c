/* 
 * File:   rfm69.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 28. Januar 2025, 19:57
 */

#include "rfm69.h"

void initRadio(void) {
    // wait a bit after POR
    _delay_ms(10);
    
    // pull reset low to turn on the module
    PORT_RFM &= ~(1 << PIN_RRST);
    
    _delay_ms(5);
    
    /*
    radioSel();
    transmit(0x39 | 0x80);
    transmit(0xaa);
    radioDes();
    */
    radioSel();
    transmit(0x01);
    uint8_t reg = transmit(0x00);
    radioDes();
    printByte(reg);
}