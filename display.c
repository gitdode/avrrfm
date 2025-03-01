/* 
 * File:   display.c
 * Author: torsten.roemer@luniks.net
 *
 * Created on 18. April 2023, 21:56
 */

#include "display.h"

void setFrame(uint16_t color) {
    fillArea(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, color);
}

width_t writeBitmap(x_t x, y_t y, uint16_t index, 
                    uint16_t bg, uint16_t fg) {
    const __flash Bitmap *bitmap = &bitmaps[index];
    setArea(x, y, bitmap->width, bitmap->height, false, false);
    writeData(bitmap->bitmap, bitmap->width, bitmap->height, 
              bitmap->space, bg, fg);
    
    return bitmap->width;
}

width_t writeGlyph(x_t x, y_t y, const __flash Font *font, code_t code, 
                   uint16_t bg, uint16_t fg) {
    const __flash Glyph *glyph = getGlyphAddress(font, code);
    setArea(x, y, glyph->width, font->height, false, false);
    writeData(glyph->bitmap, glyph->width, font->height, 
              font->space, bg, fg);
    
    return glyph->width;
}

width_t writeString(x_t x, y_t y, const __flash Font *font, const char *string, 
                    uint16_t bg, uint16_t fg) {
    width_t xorig = x;
    uint8_t offset = 0;
    for (; *string != '\0'; string++) {
        uint8_t c = (uint8_t) *string;
        if (c == 194) {
            // multibyte
        } else if (c == 195) {
            // multibyte, add 64 to get code point
            offset = 64;
        } else {
            code_t code = c + offset;
            x += writeGlyph(x, y, font, code, bg, fg);
            offset = 0;            
        }
    }
    
    return x - xorig;
}
