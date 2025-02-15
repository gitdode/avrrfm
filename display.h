/* 
 * File:   display.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 18. April 2023, 21:56
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "types.h"
#include "bitmaps.h"
#include "font.h"
#include "bitmaps.h"
#include "spi.h"
#include "tft.h"
#include "usart.h"
#include "utils.h"

/**
 * Sets the whole display to the given 16-Bit (5/6/5) RGB color.
 * 
 * @param color
 */
void setFrame(uint16_t color);

/**
 * Writes the bitmap with the given index to the given row and column 
 * and returns the width of the bitmap.
 * 
 * @param x
 * @param y
 * @param index
 * @return bitmap width
 */
width_t writeBitmap(x_t x, y_t y, uint16_t index);

/**
 * Writes the glyph with the given pseudo UTF-8 code point with the given
 * font to the given row and column and returns the width of the glyph.
 * 
 * @param x
 * @param y
 * @param font
 * @param code
 * @return glyph width
 */
width_t writeGlyph(x_t x, y_t y, const __flash Font *font, code_t code);

/**
 * Writes the given string with the given font to the given row and column.
 * 
 * @param x
 * @param y
 * @param font
 * @param string
 */
width_t writeString(x_t x, y_t y, const __flash Font *font, const char *string);

#endif /* DISPLAY_H */
