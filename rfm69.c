/* 
 * File:   rfm69.c
 * Author: torsten.roemer@luniks.net
 *
 * Created on 28. Januar 2025, 19:57
 */

#include "rfm69.h"

/**
 * Sets the module to the given operating mode.
 */
#define set_mode(mode) regWrite(OP_MODE, (regRead(OP_MODE) & ~MASK_MODE) | (mode & MASK_MODE))

static volatile uint8_t irqFlags1;
static volatile uint8_t irqFlags2;

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
    
    setFreq(kHz);
}

void setFreq(uint32_t freq) {
    freq = freq * 1000 / 61;
    
    regWrite(FRF_MSB, freq >> 16);
    regWrite(FRF_MID, freq >> 8);
    regWrite(FRF_LSB, freq >> 0);
}

void sendByte(uint8_t payload) {
    uint8_t opMode = regRead(OP_MODE);
    printString("OpMode: ");
    printByte(opMode);
    
    /*
    regWrite(NODE_ADDR, 0xaa);
    uint8_t nodeAddr = regRead(NODE_ADDR);
    printString("NodeAddr: ");
    printByte(nodeAddr);
     */
    
    // set variable packet length, turn off CRC for now
    regWrite(PCK_CFG1, (regRead(PCK_CFG1) | 0x80) & ~0x10);
    
    // set TX start condition to "at least one byte in FIFO"
    regWrite(FIFO_THRESH, regRead(FIFO_THRESH) | 0x80);
    
    regWrite(FIFO, 1); // packet length
    regWrite(FIFO, payload); // 1 byte payload
    
    // get "PacketSent" on DIO0 (default)
    regWrite(DIO_MAP1, regRead(DIO_MAP1) & ~0xc0);
    
    printString("Sending payload: ");
    printByte(payload);
    
    set_mode(MODE_TX);
    
    loop_until_bit_is_set(irqFlags2, 3);
    printString("PacketSent\r\n");
    
    set_mode(MODE_SLEEP);
}

size_t sendString(char *payload) {
    size_t len = fmin(strlen(payload), 64);
    
    regWrite(PCK_CFG1, (regRead(PCK_CFG1) | 0x80) & ~0x10);
    regWrite(FIFO_THRESH, regRead(FIFO_THRESH) | 0x80);
    
    spiSel();
    transmit(FIFO);
    transmit(len);
    for (size_t i = 0; i < len; i++) {
        transmit(payload[i]);
    }
    spiDes();
    
    printString("Sending payload...\r\n");    
    set_mode(MODE_TX);
    
    loop_until_bit_is_set(irqFlags2, 3);
    printString("PacketSent\r\n");
    
    set_mode(MODE_SLEEP);
    
    return len;
}