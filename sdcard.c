/*
 * File:   sdcard.c
 * Author: torsten.roemer@luniks.net
 *
 * Thanks to http://rjhcoding.com/avrc-sd-interface-1.php ff.
 *
 * Created on 24. Februar 2024, 00:13
 */

#include "sdcard.h"

/**
 * Transmits the given command, argument and CRC value.
 *
 * @param command
 * @param argument
 * @param crc
 */
static void command(uint8_t command, uint32_t argument, uint8_t crc) {
    // 6-bit command, start bit is always 0, transmitter bit is 1 (host command)
    transmit(command | 0x40);

    transmit(argument >> 24);
    transmit(argument >> 16);
    transmit(argument >> 8);
    transmit(argument);

    // 7-bit crc, end bit is always 1
    transmit(crc | 0x01);
}

/**
 * Reads an R1 response and returns it.
 *
 * @return R1
 */
static uint8_t readR1(void) {
    uint8_t i = 0;
    uint8_t response;

    // poll up to 8 times for response
    while ((response = transmit(0xff)) == 0xff) {
        i++;
        if (i > 8) {
            break;
        }
    }

    return response;
}

/**
 * Reads an R3/R7 response into the given array of 5 bytes.
 *
 * @param R3/R7
 */
static void readR3_7(uint8_t *response) {
    // read R1
    response[0] = readR1();

    // stop reading if R1 indicates an error
    if (response[0] > 0x01) {
        return;
    }

    response[1] = transmit(0xff);
    response[2] = transmit(0xff);
    response[3] = transmit(0xff);
    response[4] = transmit(0xff);
}

/**
 * Selects the SD card to talk to via SPI.
 */
static void sdCardSel(void) {
    PORT_SDC &= ~(1 << PIN_SDCS);
}

/**
 * Deselects the SD card to talk to via SPI.
 */
static void sdCardDes(void) {
    PORT_SDC |= (1 << PIN_SDCS);
}

/**
 * SPI selects the SD card with extra clocks before and after.
 */
static void select(void) {
    transmit(0xff);
    sdCardSel();
    transmit(0xff);
}

/**
 * SPI deselects the SD card with extra clocks before and after.
 */
static void deselect(void) {
    transmit(0xff);
    sdCardDes();
    transmit(0xff);
}

/**
 * Supplies ramp up time.
 */
static void powerOn(void) {
    _delay_ms(100);
    deselect();

    // supply at least 74 clocks
    for (uint8_t i = 0; i < 10; i++) {
        transmit(0xff);
    }
}

/**
 * Sends CMD0 to set idle SPI mode and returns the R1 response.
 *
 * @return R1
 */
static uint8_t sendIdle(void) {
    select();

    command(CMD0, CMD0_ARG, CMD0_CRC);
    uint8_t response = readR1();

    deselect();

    return response;
}

/**
 * Sends CMD8 to check version and voltage and reads the R7 response
 * into the given array of 5 bytes.
 *
 * @param R7
 */
static void sendIfCond(uint8_t *response) {
    select();

    command(CMD8, CMD8_ARG, CMD8_CRC);
    readR3_7(response);

    deselect();
}

/**
 * Sends CMD55 to tell that an app command is next and returns the R1 response.
 *
 * @return R1
 */
static uint8_t sendApp(void) {
    select();

    command(CMD55, CMD55_ARG, CMD55_CRC);
    uint8_t response = readR1();

    deselect();

    return response;
}

/**
 * Sends CMD58 to read the OCR register and reads the R3 response
 * into the given array of 5 bytes.
 *
 * @param R3
 */
static void sendOCR(uint8_t *response) {
    select();

    command(CMD58, CMD58_ARG, CMD58_CRC);
    readR3_7(response);

    deselect();
}

/**
 * Sends ACMD41 to start initialization and returns the R1 response.
 *
 * @return R1
 */
static uint8_t sendOpCond(void) {
    select();

    command(ACMD41, ACMD41_ARG, ACMD41_CRC);
    uint8_t response = readR1();

    deselect();

    return response;
}

bool initSDCard(void) {
    uint8_t response[5];

    // power on
    powerOn();

    // CMD0 - go to idle state
    response[0] = sendIdle();
    if (response[0] != 0x01) {
        // retry solves failure possibly caused by another component
        // on the same SPI bus
        response[0] = sendIdle();
    }
    if (response[0] != 0x01) {
        printString("SD card error 0\r\n");
        return false;
    }

    // CMD8 - send interface condition
    sendIfCond(response);
    if (response[0] & (1 << SD_CMD_ILLEGAL)) {
        printString("SD card is V1.x or not SD card\r\n");
        return false;
    } else if (response[0] > 0x01) {
        printString("SD card error 8\r\n");
        return false;
    } else if (response[4] != 0xaa) {
        printString("SD card echo pattern mismatch\r\n");
        return false;
    }

    // initialize to ready state
    uint8_t attempts = 0;
    do {
        if (attempts > 100) {
            printString("SD card did not become ready\r\n");
            return false;
        }

        // CMD55 - send app command
        response[0] = sendApp();
        if (response[0] < 0x02) {
            // ACMD41 - start initialization
            response[0] = sendOpCond();
        }

        _delay_ms(10);
        attempts++;
    } while (response[0] != SD_SUCCESS);

    // CMD58 - send operation conditions register
    sendOCR(response);
    if (response[0] > 0x01) {
        printString("SD card error 58\r\n");
        return false;
    } else if (!(response[1] & 0x80)) {
        printString("SD card not ready\r\n");
        return false;
    }

    printString("SD card ready\r\n");

    return true;
}

bool readSingleBlock(uint32_t address, uint8_t *block) {
    select();

    command(CMD17, address, CMD17_CRC);
    uint8_t response = readR1();
    uint8_t token = 0xff;
    bool success = false;

    if (response == SD_SUCCESS) {
        // read command was successful, wait for start block token
        for (uint16_t attempt = 0; attempt < SD_MAX_READ && token == 0xff; attempt++) {
            token = transmit(0xff);
        }

        if (token == SD_START_BLOCK) {
            // start block token received, 512 data bytes follow
            for (uint16_t i = 0; i < SD_BLOCK_SIZE; i++) {
                block[i] = transmit(0xff);
            }

            // 16-bit CRC (ignore for now)
            transmit(0xff);
            transmit(0xff);

            success = true;
        }
    }

    deselect();

    return success;
}

bool readMultiBlock(uint32_t address, Consumer consume) {
    select();

    command(CMD18, address, CMD18_CRC);
    uint8_t response = readR1();
    bool success = false;
    bool cont = false;
    uint8_t block[SD_BLOCK_SIZE];

    if (response == SD_SUCCESS) {
        // read command was successful
        do {
            // wait for start block token
            uint8_t token = 0xff;
            for (uint16_t attempt = 0; attempt < SD_MAX_READ && token == 0xff; attempt++) {
                token = transmit(0xff);
            }

            if (token == SD_START_BLOCK) {
                // start block token received, 512 data bytes follow
                for (uint16_t i = 0; i < SD_BLOCK_SIZE; i++) {
                    block[i] = transmit(0xff);
                }

                // 16-bit CRC (ignore for now)
                transmit(0xff);
                transmit(0xff);

                cont = consume(block);
            } else {
                break;
            }
        } while (cont);

        deselect();
        select();

        command(CMD12, CMD12_ARG, CMD12_CRC);
        // stuff byte on CMD12?
        transmit(0xff);
        response = readR1();

        if (response == SD_SUCCESS) {
            // wait for not busy
            uint8_t busy = 0x00;
            for (uint16_t attempt = 0; attempt < SD_MAX_READ && busy == 0x00; attempt++) {
                busy = transmit(0xff);
            }
            success = busy != 0x00;
        }
    }

    deselect();

    return success;
}

bool writeSingleBlock(uint32_t address, uint8_t *block) {
    select();

    command(CMD24, address, CMD24_CRC);
    uint8_t response = readR1();
    uint8_t token = 0xff;
    bool success = false;

    if (response == SD_SUCCESS) {
        // write command was successful, send start block token
        transmit(SD_START_BLOCK);

        // write 512 data bytes
        for (uint16_t i = 0; i < SD_BLOCK_SIZE; i++) {
            transmit(block[i]);
        }

        // wait for data response
        for (uint16_t attempt = 0; attempt < SD_MAX_WRITE && token == 0xff; attempt++) {
            token = transmit(0xff);
        }

        if ((token & 0x0f) == 0x05) {
            // data was accepted, wait while busy programming data
            for (uint16_t attempt = 0; attempt < SD_MAX_WRITE && token == 0x00; attempt++) {
                token = transmit(0xff);
            }

            success = true;
        }
    }

    deselect();

    return success;
}
