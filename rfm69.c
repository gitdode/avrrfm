/* 
 * File:   rfm69.c
 * Author: torsten.roemer@luniks.net
 *
 * Created on 28. Januar 2025, 19:57
 */

#include "rfm69.h"

/**
 * Selects the radio to talk to via SPI.
 */
static void spiSel(void) {
    PORT_RFM &= ~(1 << PIN_RCS);
}

/**
 * Deselects the radio to talk to via SPI.
 */
static void spiDes(void) {
    PORT_RFM |= (1 << PIN_RCS);
}

/**
 * Writes the given value to the given register.
 * 
 * @param reg
 * @param value
 */
static void regWrite(uint8_t reg, uint8_t value) {
    spiSel();
    transmit(reg | 0x80);
    transmit(value);
    spiDes();
}

/**
 * Reads and returns the value of the given register.
 * 
 * @param reg
 * @return value
 */
static uint8_t regRead(uint8_t reg) {
    spiSel();
    transmit(reg);
    uint8_t value = transmit(0x00);
    spiDes();
    
    return value;
}

void initRadio(uint32_t kHz) {
    // wait a bit after power on
    _delay_ms(10);
    
    // pull reset LOW to turn on the module
    PORT_RFM &= ~(1 << PIN_RRST);
    
    _delay_ms(5);
    
    setFreq(kHz);
}

void setFreq(uint32_t kHz) {
    kHz = kHz * 1000 / 61;
    
    regWrite(FRF_MSB, kHz >> 16);
    regWrite(FRF_MID, kHz >> 8);
    regWrite(FRF_LSB, kHz >> 0);
}

void doStuff(void) {
    uint8_t opMode = regRead(OP_MODE);
    printString("OpMode: ");
    printByte(opMode);
    
    regWrite(NODE_ADDR, 0xaa);
    uint8_t nodeAddr = regRead(NODE_ADDR);
    printString("NodeAddr: ");
    printByte(nodeAddr);
} 