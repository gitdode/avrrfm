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

void initRadio(uint32_t freq) {
    // wait a bit after power on
    _delay_ms(10);
    
    // pull reset LOW to turn on the module
    PORT_RFM &= ~(1 << PIN_RRST);
    
    _delay_ms(5);
    
    uint8_t version = regRead(0x10);
    printString("Version: ");
    printHex(version);
    
    // packet mode, FSK modulation, no shaping
    regWrite(DATA_MOD, 0x00);
    
    // bit rate 9.6 kBit/s
    regWrite(BITRATE_MSB, 0x0d);
    regWrite(BITRATE_LSB, 0x05);
    
    // RC calibration, must be done in standby mode (default)
    regWrite(OSC1, 0x80);
    do { } while (!(regRead(OSC1) & 0x40));
    
    // LNA 200 Ohm, gain AGC (default)
    // regWrite(0x18, 0x88);
    // reduce gain for transmitter and receiver are so close to each other
    regWrite(LNA, 0x8c);
    
    // freq of DC offset canceller and channel filter bandwith (default)
    regWrite(RX_BW, 0x55);
    
    // RX_BW during AFC (default)
    regWrite(AFC_BW, 0x8b);
    
    // turn off CLKOUT (not needed)
    regWrite(DIO_MAP2, 0x07);

    // set the carrier frequency
    freq = freq * 1000 / 61;
    regWrite(FRF_MSB, freq >> 16);
    regWrite(FRF_MID, freq >> 8);
    regWrite(FRF_LSB, freq >> 0);
    
    // enable sync word generation and detection, FIFO fill on sync address,
    // size of sync word 3
    regWrite(SYNC_CONF, 0x98);
    
    // just set all sync word values to some really creative value
    regWrite(SYNC_VAL1, 0x2f);
    regWrite(SYNC_VAL2, 0x30);
    regWrite(SYNC_VAL3, 0x31);
    regWrite(SYNC_VAL4, 0x32);
    regWrite(SYNC_VAL5, 0x33);
    regWrite(SYNC_VAL6, 0x34);
    regWrite(SYNC_VAL7, 0x35);
    regWrite(SYNC_VAL8, 0x36);

    // variable payload length, crc on, no address matching
    // regWrite(PCK_CFG1, 0x90);
    regWrite(PCK_CFG1, 0x94); // match broadcast or node address
    
    // node and broadcast address
    regWrite(NODE_ADDR, NODE_ADDRESS);
    regWrite(CAST_ADDR, CAST_ADDRESS);
    
    // set TX start condition to "at least one byte in FIFO"
    regWrite(FIFO_THRESH, 0x8f);
    
    printString("Init done\r\n");
}

void receive(void) {
    // get "PayloadReady" on DIO0
    regWrite(DIO_MAP1, 0x40);
    
    printString("Receiving...\r\n");
    setMode(MODE_RX);
    
    loop_until_bit_is_set(irqFlags2, 2);
    clearIrqFlags();
    
    setMode(MODE_STDBY);
    
    size_t len = fmin(regRead(FIFO), FIFO_SIZE) - 1;
    uint8_t address = regRead(FIFO);
    
    printString("Length:  ");
    printUint(len);
    printString("Address: ");
    printHex(address);
    printString("Payload:\r\n");
    spiSel();
    transmit(FIFO);
    for (size_t i = 0; i < len; i++) {
        printUint(transmit(FIFO));
    }
    spiDes();
}

void transmitByte(uint8_t payload) {
    spiSel();
    transmit(FIFO | 0x80);
    transmit(2);
    transmit(NODE_ADDRESS);
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

size_t transmitString(char *payload) {
    // payload + address byte
    size_t len = fmin(strlen(payload), FIFO_SIZE) + 1;
    
    spiSel();
    transmit(FIFO | 0x80);
    transmit(len);
    transmit(NODE_ADDRESS);
    for (size_t i = 0; i < len; i++) {
        transmit(payload[i]);
    }
    spiDes();
    
    // get "PacketSent" on DIO0 (default)
    regWrite(DIO_MAP1, 0x00);
    
    printString("Sending...\r\n");    
    setMode(MODE_TX);
    
    loop_until_bit_is_set(irqFlags2, 3);
    clearIrqFlags();
    printString("PacketSent\r\n");
    
    setMode(MODE_STDBY);
    
    return len;
}
