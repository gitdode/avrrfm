/* 
 * File:   colorspace.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 3. Juli 2024, 19:02
 */

#ifndef COLORSPACE_H
#define COLORSPACE_H

// TODO use enum? typedef?
#define SPACE_MONO1  1
#define SPACE_GREY4  4
#define SPACE_RGB16  16

#include <stdint.h>
#include "types.h"
#include "spi.h"

/**
 * Helper to write image data to the display, converting from the given 
 * color space to that of the display.
 * 
 * @param bitmap pointer to bitmap data in program memory
 * @param width width of the bitmap in pixels
 * @param height height of the bitmap in pixels
 * @param space color space of the bitmap
 */
void writeSpace(const __flash uint8_t *bitmap,
                width_t width, height_t height,
                space_t space);

#endif /* COLORSPACE_H */

