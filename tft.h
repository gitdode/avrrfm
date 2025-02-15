/* 
 * File:   tft.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 6. November 2023, 18:45
 */

#ifndef TFT_H
#define TFT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <util/delay.h>
#include "types.h"
#include "pins.h"
#include "usart.h"
#include "spi.h"
#include "colorspace.h"

#define SWRESET 0x01
#define SLPIN   0x10
#define SLPOUT  0x11
#define NORON   0x13
#define INVOFF  0x20
#define INVON   0x21
#define DISPON  0x29
#define CASET   0x2a
#define RASET   0x2b
#define RAMWR   0x2c
#define MADCTL  0x36
#define COLMOD  0x3a

#ifndef DISPLAY_WIDTH
    #define DISPLAY_WIDTH   160
#endif

#ifndef DISPLAY_HEIGHT
    #define DISPLAY_HEIGHT  128
#endif

#ifndef BGR
    #define BGR     0
#endif

#ifndef INVERT
    #define INVERT  0
#endif

#ifndef HFLIP
    #define HFLIP   0
#endif

#ifndef VFLIP
    #define VFLIP   0
#endif

/**
 * Initializes the display.
 */
void initDisplay(void);

/**
 * Displays a demo.
 */
void demoDisplay(void);

/**
 * Draws a pixel at given coordinates and color.
 * 
 * @param x
 * @param y
 * @param color
 */
void drawPixel(x_t x, y_t y, uint16_t color);

/**
 * Draws a circle at given center coordinates, with given radius and color.
 * 
 * @param x
 * @param y
 * @param radius
 * @param color
 */
void drawCircle(x_t x, y_t y, uint16_t radius, uint16_t color);

/**
 * Draws a rectangle at given origin, with given width, height and color.
 * 
 * @param x
 * @param y
 * @param width
 * @param height
 * @param color
 */
void drawRectangle(x_t x, y_t y, width_t width, height_t height, 
                   uint16_t color);

/**
 * Sets to write data to display RAM.
 */
void writeStart(void);

/**
 * Restart writing to display after SPI deselecting it.
 */
void writeRestart(void);

/**
 * Writes the given byte to display RAM.
 * 
 * @param byte
 */
void writeByte(uint8_t byte);

/**
 * Completes writing data to display RAM.
 */
void writeEnd(void);

/**
 * Sets the given color in the given area of the display.
 * 
 * @param x in pixels, origin top left
 * @param y in pixels, origin top left
 * @param width width in pixels
 * @param height height in pixels
 * @param color 16-Bit (5/6/5) RGB color
 */
void fillArea(x_t x, y_t y,
              width_t width, height_t height,
              uint16_t color);

/**
 * Sets the area to write image data to.
 * 
 * @param x in pixels, origin top left
 * @param y in pixels, origin top left
 * @param width width of the bitmap in pixels
 * @param height height of the bitmap in pixels
 * @param hflip if image should be flipped horizontally
 * @param vflip if image should be flipped vertically
 */
void setArea(x_t x, y_t y,
             width_t width, height_t height,
             bool hflip, bool vflip);

/**
 * Writes image data to the previously set area.
 * 
 * @param bitmap pointer to bitmap data in program memory
 * @param width width of the bitmap in pixels
 * @param height height of the bitmap in pixels
 * @param space color space of the bitmap
 */
void writeData(const __flash uint8_t *bitmap,
               width_t width, height_t height,
               space_t space);

#endif /* TFT_H */
