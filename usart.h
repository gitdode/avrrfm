/*
 * usart.h
 *
 *  Created on: 19.05.2015
 *      Author: dode@luniks.net
 */

#ifndef USART_H_
#define USART_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>
#include "utils.h"

#define USART_LENGTH 128

#ifndef BAUD
#define BAUD 9600
#endif

/**
 * Sets the baudrate and enables the transmitter and receiver.
 */
void initUSART(void);

/**
 * Returns true if a CR or LF terminated line of data was received via USART.
 * 
 * @return data was received
 */
bool isUSARTReceived(void);

/**
 * Appends the data received via USART to the given string with the given
 * length.
 */
void getUSARTData(char *data, size_t length);

/**
 * Prints the given string via USART.
 */
void printString(const char *data);

/**
 * Prints the given integer including CR + LF via USART.
 */
void printInt(int8_t data);

/**
 * Prints the given unsigned integer in hex notation including CR + LF 
 * via USART.
 */
void printHex(uint8_t data);

/**
 * Prints the given unsigned integer in binary notation including CR + LF
 * via USART.
 */
void printByte(uint8_t data);

#endif /* USART_H_ */
