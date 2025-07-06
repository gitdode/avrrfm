/* 
 * File:   tft.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 6. November 2023, 18:45
 */

#ifndef LIBTFT_H
#define LIBTFT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "font.h"

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

/**
 * F_CPU dependent delay of 100 milliseconds.
 * _delay_ms(100);
 */
void _tftDelay100(void);

/**
 * Selects the display to talk to via SPI.
 * PORTB &= ~(1 << PB1);
 */
void _tftSel(void);

/**
 * Deselects the display to talk to via SPI.
 * PORTB |= (1 << PB1);
 */
void _tftDes(void);

/**
 * Sets display to send a command.
 * PORTB &= ~(1 << PB2);
 */
void _tftSetCmd(void);

/**
 * Sets display to send data.
 * PORTB |= (1 << PB2);
 */
void _tftSetData(void);

/**
 * Does a hardware reset.
 * PORTB &= ~(1 << PB3);
 * _delay_ms(10);
 * PORTB |= (1 << PB3);
 */
void _tftReset(void);

/**
 * SPI transmits/receives given data/returns it.
 * 
 * @param data
 * @return data
 */
uint8_t _tftTx(uint8_t data);

/**
 * Initializes the display.
 * 
 * @param width display width
 * @param height display height
 * @param hflip horizontal flip
 * @param vflip vertical flip
 * @param bgr BGR or RGB
 * @param invert invert display
 */
void tftInit(width_t width, height_t height,
             bool hflip, bool vflip,
             bool bgr, bool invert);

/**
 * Sets to write data to display RAM.
 */
void tftWriteStart(void);

/**
 * Restart writing to display after SPI deselecting it.
 */
void tftWriteRestart(void);

/**
 * Writes the given byte to display RAM.
 * 
 * @param byte
 */
void tftWriteByte(uint8_t byte);

/**
 * Completes writing data to display RAM.
 */
void tftWriteEnd(void);

/**
 * Sets the given color in the given area of the display.
 * 
 * @param x in pixels, origin top left
 * @param y in pixels, origin top left
 * @param width width in pixels
 * @param height height in pixels
 * @param color 16-Bit (5/6/5) RGB color
 */
void tftFillArea(x_t x, y_t y,
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
void tftSetArea(x_t x, y_t y,
             width_t width, height_t height,
             bool hflip, bool vflip);

/**
 * Sets the whole display to the given 16-Bit (5/6/5) RGB color.
 * 
 * @param color
 */
void tftSetFrame(uint16_t color);

/**
 * Helper to write image data to the display, converting from the given 
 * color space to that of the display.
 * 
 * @param bitmap pointer to bitmap data in program memory
 * @param width width of the bitmap in pixels
 * @param height height of the bitmap in pixels
 * @param space color space of the bitmap
 * @param bg background color (used only with SPACE_MONO1)
 * @param fg foreground color (used only with SPACE_MONO1)
 */
void tftWriteSpace(const __flash uint8_t *bitmap,
                width_t width, height_t height,
                space_t space, uint16_t bg, uint16_t fg);

/**
 * Writes image data to the previously set area.
 * 
 * @param bitmap pointer to bitmap data in program memory
 * @param width width of the bitmap in pixels
 * @param height height of the bitmap in pixels
 * @param space color space of the bitmap
 * @param bg background color (used only with SPACE_MONO1)
 * @param fg foreground color (used only with SPACE_MONO1)
 */
void tftWriteData(const __flash uint8_t *bitmap,
               width_t width, height_t height,
               space_t space, uint16_t bg, uint16_t fg);

/**
 * Writes the bitmap with the given index to the given row and column 
 * and returns the width of the bitmap.
 * 
 * @param x
 * @param y
 * @param index
 * @param bg background color (used only with SPACE_MONO1)
 * @param fg foreground color (used only with SPACE_MONO1)
 * @return bitmap width
 */
width_t tftWriteBitmap(x_t x, y_t y, uint16_t index, 
                       uint16_t bg, uint16_t fg);

/**
 * Writes the glyph with the given pseudo UTF-8 code point with the given
 * font to the given row and column and returns the width of the glyph.
 * 
 * @param x
 * @param y
 * @param font
 * @param code
 * @param bg background color (used only with SPACE_MONO1)
 * @param fg foreground color (used only with SPACE_MONO1)
 * @return glyph width
 */
width_t tftWriteGlyph(x_t x, y_t y, const __flash Font *font, code_t code, 
                      uint16_t bg, uint16_t fg);

/**
 * Writes the given string with the given font to the given row and column 
 * and returns the width of the string.
 * 
 * @param x
 * @param y
 * @param font
 * @param string
 * @param bg background color (used only with SPACE_MONO1)
 * @param fg foreground color (used only with SPACE_MONO1)
 * @return glyph width
 */
width_t tftWriteString(x_t x, y_t y, const __flash Font *font, const char *string, 
                       uint16_t bg, uint16_t fg);

#endif /* LIBTFT_H */
