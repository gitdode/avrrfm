/* 
 * File:   bitmaps.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 16. April 2023, 18:19
 */

#ifndef BITMAPS_H
#define BITMAPS_H

#include <stdio.h>
#include <stdint.h>
#include "types.h"
#include "colorspace.h"

/**
 * A bitmap with its width and height, and data.
 */
typedef struct {
    /** Width of the bitmap. */
    const width_t width;
    /** Height of the bitmap. */
    const height_t height;
    /** Color space of the bitmap. */
    const space_t space;
    /** The actual bitmap. */
    const __flash uint8_t *bitmap;
} Bitmap;

/**
 * Available bitmaps.
 */
extern const __flash Bitmap bitmaps[];

#endif /* BITMAPS_H */

