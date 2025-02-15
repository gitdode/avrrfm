/* 
 * File:   colorspace.c
 * Author: torsten.roemer@luniks.net
 *
 * Created on 3. Juli 2024, 19:03
 */

#include "colorspace.h"

/**
 * Converts the given 8 pixel in 1-Bit monochrome to 16-Bit RGB (5/6/5) color
 * stored in the given array of 16 bytes.
 * 
 * @param mono 8 pixel in 1-Bit monochrome
 * @param rgb 8 pixel in 16-Bit RGB (5/6/5) color
 */
static void mono1ToRGB16(uint8_t mono, uint8_t *rgb) {
    for (uint8_t i = 0; i < 16; i++) {
        rgb[i] = (mono & (1 << ((15 - i) >> 1))) ? 0x0 : 0xff;
    }
}

/*
 * Converts the given two pixel in 4-Bit greyscale to 16-Bit RGB (5/6/5) color
 * stored in the given array of four bytes.
 * 
 * @param grey two pixel in 4-Bit greyscale
 * @param rgb two pixel in 16-Bit RGB (5/6/5) color
 */
static void grey4ToRGB16(uint8_t grey, uint8_t *rgb) {
    uint8_t grey4 = ((grey >> 4) & 1);
    uint8_t grey0 = ((grey >> 0) & 1);
    
    rgb[0] = (grey & 0xf0);
    rgb[0] |= (grey4 << 3);
    rgb[0] |= (grey >> 5);

    rgb[1] = ((grey & 0xf0) << 3);
    rgb[1] |= ((grey & 0xf0) >> 3);
    rgb[1] |= (grey4 << 6) | (grey4 << 5) | (grey4 << 0);

    rgb[2] = (grey << 4);
    rgb[2] |= (grey0 << 3);
    rgb[2] |= ((grey & 0x0f) >> 1);

    rgb[3] = (grey << 7);
    rgb[3] |= ((grey & 0x0f) << 1);
    rgb[3] |= (grey0 << 6) | (grey0 << 5) | (grey0 << 0);    
}

void writeSpace(const __flash uint8_t *bitmap,
                width_t width, height_t height,
                space_t space) {
    switch (space) {
        case SPACE_MONO1: {
            bytes_t bytes = width * height / 8;
            for (uint16_t i = 0; i < bytes; i++) {
                uint8_t rgb[16];
                mono1ToRGB16(bitmap[i], rgb);
                for (uint8_t j = 0; j < 16; j++) {
                    transmit(rgb[j]);
                }
            }            
        }; break;
        case SPACE_GREY4: {
            bytes_t bytes = width * height / 2;
            for (uint16_t i = 0; i < bytes; i++) {
                uint8_t rgb[4];
                grey4ToRGB16(bitmap[i], rgb);
                for (uint8_t j = 0; j < 4; j++) {
                    transmit(rgb[j]);
                }
            }
        }; break;
        default: {
            // SPACE_RGB16
            bytes_t bytes = width * height * 2;
            for (uint16_t i = 0; i < bytes; i++) {
                transmit(bitmap[i]);
            }
        }
    }    
}