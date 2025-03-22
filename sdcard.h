/*
 * File:   sdcard.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 24. Februar 2024, 00:13
 */

#ifndef SDCARD_H
#define SDCARD_H

#include <stdbool.h>
#include <util/delay.h>
#include "pins.h"
#include "spi.h"
#include "usart.h"
#include "types.h"

#define CMD0        0
#define CMD0_ARG    0x00000000
#define CMD0_CRC    0x94

#define CMD8        8
#define CMD8_ARG    0x0000001aa
#define CMD8_CRC    0x86

#define CMD12       12
#define CMD12_ARG   0x00000000
#define CMD12_CRC   0x60

#define CMD17       17
#define CMD17_CRC   0x00

#define CMD18       18
#define CMD18_CRC   0x00

#define CMD24       24
#define CMD24_CRC   0x00

#define CMD55       55
#define CMD55_ARG   0x00000000
#define CMD55_CRC   0x64

#define CMD58       58
#define CMD58_ARG   0x00000000
#define CMD58_CRC   0xfc

#define ACMD41      41
#define ACMD41_ARG  0x40000000
#define ACMD41_CRC  0x76

// TODO depends on SPI SCK frequency
#define SD_MAX_READ     50000 // SPI clock ticks in 100 ms
#define SD_MAX_WRITE    125000 // SPI clock ticks in 250 ms

#define SD_BLOCK_SIZE   512

#define SD_CMD_ILLEGAL  2

#define SD_SUCCESS      0x00
#define SD_START_BLOCK  0xfe

/**
 * Initializes the SD Card and returns true on success, false otherwise.
 *
 * @return true on success, false otherwise
 */
bool initSDCard(void);

/**
 * Reads a single block of 512 bytes at the given address into
 * the given buffer and returns true on success, false otherwise.
 *
 * @param address address in 512 byte units
 * @param block 512 byte buffer
 * @return success
 */
bool readSingleBlock(uint32_t address, uint8_t *block);

/**
 * Reads multiple blocks of 512 bytes starting at the given address
 * and, for each block, passes the buffer to the given function.
 * By returning false, the function can request to stop reading blocks.
 *
 * @param address
 * @param consume
 * @return success
 */
bool readMultiBlock(uint32_t address, Consumer consume);

/**
 * Writes a single block of 512 bytes starting at the given address
 * from the given buffer and returns true on success, false otherwise.
 *
 * @param address address in 512 byte units
 * @param block 512 byte buffer
 * @return success
 */
bool writeSingleBlock(uint32_t address, uint8_t *block);

#endif /* SDCARD_H */
