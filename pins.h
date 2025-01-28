/* 
 * File:   pins.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 1. April 2023, 19:25
 */

#ifndef PINS_H
#define PINS_H

#include <avr/io.h>

/* SPI */
#define DDR_SPI   DDRB
#define PORT_SPI  PORTB
#define PIN_SS    PB2
#define PIN_MOSI  PB3
#define PIN_MISO  PB4
#define PIN_SCK   PB5

/* I2C */
#define DDR_I2C   DDRC
#define PORT_I2C  PORTC
#define PIN_SCL   PC5
#define PIN_SDA   PC4

/* Radio Int */
#define PIN_RINT  PD2 // radio interrupt

/* Radio SPI */
#define DDR_RFM   DDRB
#define PORT_RFM  PORTB
#define PIN_RCS   PB1 // radio chip select
#define PIN_RRST  PB0 // radio reset

#endif /* PINS_H */
