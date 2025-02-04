/* 
 * File:   rfm69.c
 * Author: torsten.roemer@luniks.net
 *
 * Created on 28. Januar 2025, 19:57
 */

#include "rfm69.h"

static volatile uint8_t irqFlags1 = 0;
static volatile uint8_t irqFlags2 = 0;

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

/**
 * Sets the module to the given operating mode.
 */
static void setMode(uint8_t mode) {
    regWrite(OP_MODE, (regRead(OP_MODE) & ~MASK_MODE) | (mode & MASK_MODE));
}

/**
 * Clears the IRQ flags read from the module.
 */
static void clearIrqFlags(void) {
    irqFlags1 = 0;
    irqFlags2 = 0;
}

ISR(INT0_vect) {
    irqFlags1 = regRead(IRQ_FLAGS1);
    irqFlags2 = regRead(IRQ_FLAGS2);
    // printString("irq\r\n");
}

void initRadio(uint32_t kHz) {
    // wait a bit after power on
    _delay_ms(10);
    
    // pull reset LOW to turn on the module
    PORT_RFM &= ~(1 << PIN_RRST);
    
    _delay_ms(5);
    
    // turn off CLKOUT
    regWrite(DIO_MAP2, 0x07);
    
    setFreq(kHz);
    // regWrite(NODE_ADDR, 0x42);
    
    // add initialization so both modules actually talk to each other :-)
}

void setFreq(uint32_t freq) {
    freq = freq * 1000 / 61;
    
    regWrite(FRF_MSB, freq >> 16);
    regWrite(FRF_MID, freq >> 8);
    regWrite(FRF_LSB, freq >> 0);
}

void transmitByte(uint8_t payload) {
    // set variable packet length, turn off CRC for now
    regWrite(PCK_CFG1, 0x80);
    
    // set TX start condition to "at least one byte in FIFO"
    regWrite(FIFO_THRESH, 0x8f);
    
    spiSel();
    transmit(FIFO | 0x80);
    transmit(2); // TODO includes address?
    transmit(0x42);
    transmit(payload);
    spiDes();
    
    // get "PacketSent" on DIO0 (default)
    regWrite(DIO_MAP1, 0x00);
    
    setMode(MODE_TX);
    
    loop_until_bit_is_set(irqFlags2, 3);
    clearIrqFlags();

    setMode(MODE_STDBY);
    
    printString("PacketSent\r\n");
}

uint8_t receiveByte(void) {
    // set variable packet length, turn off CRC for now, address filtering
    regWrite(PCK_CFG1, 0x84);
    // regWrite(PCK_CFG1, 0x80);
    
    // get "PayloadReady" on DIO0
    regWrite(DIO_MAP1, 0x40);
    
    printString("Starting receiving...\r\n");
    setMode(MODE_RX);
    
    loop_until_bit_is_set(irqFlags2, 2);
    clearIrqFlags();
    
    setMode(MODE_STDBY);
    
    // can also read FIFO after going to standby
    printByte(regRead(FIFO));
    printByte(regRead(FIFO));
    printByte(regRead(FIFO));
    
    return 0;
}

size_t transmitString(char *payload) {
    size_t len = fmin(strlen(payload), 64);
    
    regWrite(PCK_CFG1, 0x80);
    regWrite(FIFO_THRESH, 0x8f);
    
    spiSel();
    transmit(FIFO);
    transmit(len);
    for (size_t i = 0; i < len; i++) {
        transmit(payload[i]);
    }
    spiDes();
    
    // get "PacketSent" on DIO0 (default)
    regWrite(DIO_MAP1, 0x00);
    
    printString("Sending payload...\r\n");    
    setMode(MODE_TX);
    
    loop_until_bit_is_set(irqFlags2, 3);
    clearIrqFlags();
    printString("PacketSent\r\n");
    
    setMode(MODE_STDBY);
    
    return len;
}
