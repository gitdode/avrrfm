/* 
 * File:   dejavu.c
 * Author: torsten.roemer@luniks.net
 * https://dejavu-fonts.github.io/License.html
 *
 * Created on 23. April 2023, 18:41
 */

#include <stdio.h>
#include "font.h"
#include "dejavu.h"
#include "utils.h"

#define HEIGHT 56

static const __flash uint8_t SPACE[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t EXCLAMATION_MARK[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x03, 0xc0, 0x03, 0xc0,
    0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0,
    0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0,
    0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0,
    0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0,
    0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0,
    0x03, 0xc0, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xc0,
    0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0, 0x03, 0xc0,
    0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t NUMBER_SIGN[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x03, 0xc0, 0xf0, 0x00, 0x03, 0xc0, 0xe0,
    0x00, 0x03, 0xc1, 0xe0, 0x00, 0x07, 0x81, 0xe0,
    0x00, 0x07, 0x81, 0xe0, 0x00, 0x07, 0x81, 0xc0,
    0x00, 0x07, 0x83, 0xc0, 0x00, 0x0f, 0x03, 0xc0,
    0x00, 0x0f, 0x03, 0xc0, 0x00, 0x0f, 0x03, 0x80,
    0x07, 0xff, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xfe,
    0x07, 0xff, 0xff, 0xfe, 0x07, 0xff, 0xff, 0xfe,
    0x00, 0x1e, 0x0f, 0x00, 0x00, 0x1c, 0x0f, 0x00,
    0x00, 0x3c, 0x0f, 0x00, 0x00, 0x3c, 0x0e, 0x00,
    0x00, 0x3c, 0x1e, 0x00, 0x00, 0x78, 0x1e, 0x00,
    0x00, 0x78, 0x1e, 0x00, 0x3f, 0xff, 0xff, 0xf0,
    0x3f, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xf0,
    0x3f, 0xff, 0xff, 0xf0, 0x00, 0xf0, 0x38, 0x00,
    0x00, 0xf0, 0x78, 0x00, 0x01, 0xe0, 0x78, 0x00,
    0x01, 0xe0, 0x78, 0x00, 0x01, 0xe0, 0x70, 0x00,
    0x01, 0xe0, 0xf0, 0x00, 0x03, 0xc0, 0xf0, 0x00,
    0x03, 0xc0, 0xf0, 0x00, 0x03, 0xc0, 0xe0, 0x00,
    0x03, 0xc1, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t PERCENT_SIGN[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00,
    0x03, 0xfe, 0x00, 0x00, 0x07, 0xff, 0x00, 0x00, 
    0x0f, 0x07, 0x80, 0x00, 0x0e, 0x03, 0x80, 0x00,
    0x1c, 0x01, 0xc0, 0x00, 0x1c, 0x01, 0xc0, 0x00, 
    0x1c, 0x01, 0xc0, 0x00, 0x1c, 0x01, 0xc0, 0x00,
    0x1c, 0x01, 0xc0, 0x00, 0x0e, 0x03, 0x80, 0x00, 
    0x0f, 0x07, 0x80, 0x18, 0x07, 0xff, 0x00, 0xf8,
    0x03, 0xfe, 0x03, 0xf0, 0x00, 0xf8, 0x1f, 0xc0, 
    0x00, 0x00, 0x7e, 0x00, 0x00, 0x03, 0xf8, 0x00,
    0x00, 0x0f, 0xc0, 0x00, 0x00, 0x3f, 0x00, 0x00, 
    0x01, 0xf8, 0x0f, 0x80, 0x07, 0xe0, 0x3f, 0xe0,
    0x0f, 0x80, 0x7f, 0xf0, 0x04, 0x00, 0xf0, 0x78, 
    0x00, 0x00, 0xe0, 0x38, 0x00, 0x01, 0xc0, 0x1c,
    0x00, 0x01, 0xc0, 0x1c, 0x00, 0x01, 0xc0, 0x1c, 
    0x00, 0x01, 0xc0, 0x1c, 0x00, 0x01, 0xc0, 0x1c,
    0x00, 0x00, 0xe0, 0x38, 0x00, 0x00, 0xf0, 0x78, 
    0x00, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0x3f, 0xe0,
    0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t ASTERISK[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x38,
    0x00, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x20,
    0x38, 0x08, 0x78, 0x38, 0x3c, 0x7c, 0x38, 0x7c,
    0x1f, 0x39, 0xf0, 0x07, 0xbb, 0xc0, 0x01, 0xff,
    0x00, 0x00, 0xfe, 0x00, 0x00, 0xfe, 0x00, 0x01,
    0xff, 0x00, 0x07, 0xbb, 0xc0, 0x1f, 0x39, 0xf0,
    0x7c, 0x38, 0x7c, 0x78, 0x38, 0x3c, 0x20, 0x38,
    0x08, 0x00, 0x38, 0x00, 0x00, 0x38, 0x00, 0x00,
    0x38, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t PLUS_SIGN[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 
    0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00,
    0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 
    0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c,
    0x00, 0x00, 0x3c, 0x00, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 
    0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00,
    0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 
    0x00, 0x3c, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t COMMA[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x07, 0xe0,
    0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0,
    0x07, 0xc0, 0x0f, 0xc0, 0x0f, 0x80, 0x0f, 0x80,
    0x0f, 0x80, 0x0f, 0x00, 0x1f, 0x00, 0x1e, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t HYPHEN_MINUS[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x03, 0xff, 0xc0, 0x03, 0xff, 0xc0,
    0x03, 0xff, 0xc0, 0x03, 0xff, 0xc0, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t FULL_STOP[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x07, 0xe0,
    0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0,
    0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t SOLIDUS[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x07, 0x80,
    0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x1e, 0x00,
    0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1e, 0x00,
    0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x3c, 0x00,
    0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x78, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00,
    0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00,
    0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00,
    0x00, 0x07, 0x80, 0x00, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00,
    0x00, 0x1e, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00,
    0x00, 0x3c, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00,
    0x00, 0x78, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00,
    0x00, 0xf0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00,
    0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00,
    0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00,
    0x07, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00,
    0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t DIGIT_0[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00,
    0x00, 0x1f, 0xfc, 0x00, 0x00, 0x3f, 0xfe, 0x00,
    0x00, 0x7f, 0xff, 0x00, 0x00, 0xfc, 0x1f, 0x80,
    0x01, 0xf0, 0x07, 0xc0, 0x01, 0xf0, 0x07, 0xc0,
    0x01, 0xe0, 0x03, 0xc0, 0x03, 0xc0, 0x01, 0xe0,
    0x03, 0xc0, 0x01, 0xe0, 0x03, 0xc0, 0x01, 0xe0,
    0x03, 0xc0, 0x01, 0xe0, 0x07, 0x80, 0x00, 0xf0,
    0x07, 0x80, 0x00, 0xf0, 0x07, 0x80, 0x00, 0xf0,
    0x07, 0x80, 0x00, 0xf0, 0x07, 0x80, 0x00, 0xf0,
    0x07, 0x80, 0x00, 0xf0, 0x07, 0x80, 0x00, 0xf0,
    0x07, 0x80, 0x00, 0xf0, 0x07, 0x80, 0x00, 0xf0,
    0x07, 0x80, 0x00, 0xf0, 0x07, 0x80, 0x00, 0xf0,
    0x07, 0x80, 0x00, 0xf0, 0x07, 0x80, 0x00, 0xf0,
    0x03, 0xc0, 0x01, 0xe0, 0x03, 0xc0, 0x01, 0xe0,
    0x03, 0xc0, 0x01, 0xe0, 0x03, 0xc0, 0x01, 0xe0,
    0x01, 0xe0, 0x03, 0xc0, 0x01, 0xf0, 0x07, 0xc0,
    0x01, 0xf0, 0x07, 0xc0, 0x00, 0xfc, 0x1f, 0x80,
    0x00, 0x7f, 0xff, 0x00, 0x00, 0x3f, 0xfe, 0x00,
    0x00, 0x1f, 0xfc, 0x00, 0x00, 0x07, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t DIGIT_1[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x03, 0xf0, 0x00, 0x00, 0x3f, 0xf0, 0x00,
    0x00, 0xff, 0xf0, 0x00, 0x00, 0xff, 0xf0, 0x00,
    0x00, 0xfc, 0xf0, 0x00, 0x00, 0xc0, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x7f, 0xff, 0xe0,
    0x00, 0x7f, 0xff, 0xe0, 0x00, 0x7f, 0xff, 0xe0,
    0x00, 0x7f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t DIGIT_2[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf8, 0x00,
    0x01, 0xff, 0xff, 0x00, 0x03, 0xff, 0xff, 0x80,
    0x03, 0xff, 0xff, 0xc0, 0x03, 0xf8, 0x07, 0xe0,
    0x03, 0xc0, 0x01, 0xf0, 0x02, 0x00, 0x00, 0xf0,
    0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x78,
    0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x78,
    0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x78,
    0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0xf0,
    0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x03, 0xe0,
    0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x0f, 0xc0,
    0x00, 0x00, 0x1f, 0x80, 0x00, 0x00, 0x3f, 0x00,
    0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0xfc, 0x00,
    0x00, 0x01, 0xf8, 0x00, 0x00, 0x03, 0xf0, 0x00,
    0x00, 0x07, 0xe0, 0x00, 0x00, 0x0f, 0xc0, 0x00,
    0x00, 0x1f, 0x80, 0x00, 0x00, 0x3f, 0x00, 0x00,
    0x00, 0x7e, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00,
    0x01, 0xf0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xf8,
    0x03, 0xff, 0xff, 0xf8, 0x03, 0xff, 0xff, 0xf8,
    0x03, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t DIGIT_3[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00,
    0x00, 0xff, 0xfc, 0x00, 0x03, 0xff, 0xff, 0x00,
    0x03, 0xff, 0xff, 0x80, 0x03, 0xe0, 0x1f, 0xc0,
    0x02, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x03, 0xe0,
    0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
    0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
    0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x03, 0xc0,
    0x00, 0x00, 0x07, 0xc0, 0x00, 0x00, 0x1f, 0x80,
    0x00, 0x1f, 0xff, 0x00, 0x00, 0x1f, 0xfe, 0x00,
    0x00, 0x1f, 0xfe, 0x00, 0x00, 0x1f, 0xff, 0x00,
    0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x07, 0xc0,
    0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x01, 0xe0,
    0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0,
    0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0,
    0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0,
    0x00, 0x00, 0x01, 0xf0, 0x04, 0x00, 0x03, 0xe0,
    0x07, 0x00, 0x07, 0xe0, 0x07, 0xe0, 0x1f, 0xc0,
    0x07, 0xff, 0xff, 0x80, 0x07, 0xff, 0xff, 0x00,
    0x03, 0xff, 0xfe, 0x00, 0x00, 0x3f, 0xf0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t DIGIT_4[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x7e, 0x00,
    0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xfe, 0x00,
    0x00, 0x01, 0xde, 0x00, 0x00, 0x03, 0xde, 0x00,
    0x00, 0x03, 0x9e, 0x00, 0x00, 0x07, 0x9e, 0x00,
    0x00, 0x0f, 0x1e, 0x00, 0x00, 0x0f, 0x1e, 0x00,
    0x00, 0x1e, 0x1e, 0x00, 0x00, 0x1c, 0x1e, 0x00,
    0x00, 0x3c, 0x1e, 0x00, 0x00, 0x78, 0x1e, 0x00,
    0x00, 0x78, 0x1e, 0x00, 0x00, 0xf0, 0x1e, 0x00,
    0x00, 0xf0, 0x1e, 0x00, 0x01, 0xe0, 0x1e, 0x00,
    0x03, 0xc0, 0x1e, 0x00, 0x03, 0xc0, 0x1e, 0x00,
    0x07, 0x80, 0x1e, 0x00, 0x07, 0x80, 0x1e, 0x00,
    0x0f, 0x00, 0x1e, 0x00, 0x0f, 0xff, 0xff, 0xf0,
    0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf0,
    0x0f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x1e, 0x00,
    0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1e, 0x00,
    0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1e, 0x00,
    0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1e, 0x00,
    0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t DIGIT_5[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xc0,
    0x01, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xc0,
    0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00,
    0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00,
    0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00,
    0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00,
    0x01, 0xef, 0xf0, 0x00, 0x01, 0xff, 0xfc, 0x00,
    0x01, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0x80,
    0x01, 0xe0, 0x1f, 0xc0, 0x01, 0x00, 0x07, 0xc0,
    0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x01, 0xe0,
    0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x00, 0xf0,
    0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0,
    0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0,
    0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x01, 0xf0,
    0x00, 0x00, 0x01, 0xe0, 0x04, 0x00, 0x03, 0xe0,
    0x07, 0x00, 0x07, 0xc0, 0x07, 0xe0, 0x1f, 0xc0,
    0x07, 0xff, 0xff, 0x80, 0x07, 0xff, 0xff, 0x00,
    0x03, 0xff, 0xfc, 0x00, 0x00, 0x7f, 0xe0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t DIGIT_6[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00,
    0x00, 0x0f, 0xff, 0x80, 0x00, 0x3f, 0xff, 0x80,
    0x00, 0x7f, 0xff, 0x80, 0x00, 0x7e, 0x01, 0x80,
    0x00, 0xf8, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00,
    0x01, 0xe0, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00,
    0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00,
    0x03, 0xc0, 0x00, 0x00, 0x07, 0x80, 0x00, 0x00,
    0x07, 0x83, 0xf8, 0x00, 0x07, 0x8f, 0xfe, 0x00,
    0x07, 0x9f, 0xff, 0x00, 0x07, 0xbf, 0xff, 0x80,
    0x07, 0xfc, 0x0f, 0xc0, 0x07, 0xf8, 0x03, 0xe0,
    0x07, 0xf0, 0x01, 0xe0, 0x07, 0xe0, 0x01, 0xe0,
    0x07, 0xe0, 0x00, 0xf0, 0x07, 0xc0, 0x00, 0xf0,
    0x07, 0xc0, 0x00, 0xf0, 0x07, 0xc0, 0x00, 0xf0,
    0x07, 0xc0, 0x00, 0xf0, 0x03, 0xc0, 0x00, 0xf0,
    0x03, 0xc0, 0x00, 0xf0, 0x03, 0xe0, 0x00, 0xf0,
    0x03, 0xe0, 0x01, 0xe0, 0x01, 0xf0, 0x01, 0xe0,
    0x01, 0xf8, 0x03, 0xe0, 0x00, 0xfc, 0x0f, 0xc0,
    0x00, 0x7f, 0xff, 0x80, 0x00, 0x3f, 0xff, 0x00,
    0x00, 0x1f, 0xfe, 0x00, 0x00, 0x07, 0xf8, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t DIGIT_7[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x07, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xf0,
    0x07, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xe0,
    0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x03, 0xe0,
    0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x07, 0xc0,
    0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x07, 0x80,
    0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x1e, 0x00,
    0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3c, 0x00,
    0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x7c, 0x00,
    0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0xf8, 0x00,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0, 0x00,
    0x00, 0x03, 0xe0, 0x00, 0x00, 0x03, 0xc0, 0x00,
    0x00, 0x03, 0xc0, 0x00, 0x00, 0x07, 0x80, 0x00,
    0x00, 0x07, 0x80, 0x00, 0x00, 0x0f, 0x80, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00,
    0x00, 0x1e, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00,
    0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t DIGIT_8[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xf0, 0x00,
    0x00, 0x3f, 0xfe, 0x00, 0x00, 0x7f, 0xff, 0x00,
    0x00, 0xff, 0xff, 0x80, 0x01, 0xf8, 0x0f, 0xc0,
    0x01, 0xf0, 0x07, 0xc0, 0x03, 0xe0, 0x03, 0xe0,
    0x03, 0xc0, 0x01, 0xe0, 0x03, 0xc0, 0x01, 0xe0,
    0x03, 0xc0, 0x01, 0xe0, 0x03, 0xc0, 0x01, 0xe0,
    0x03, 0xc0, 0x01, 0xe0, 0x01, 0xe0, 0x03, 0xc0,
    0x01, 0xf0, 0x07, 0xc0, 0x00, 0xf8, 0x0f, 0x80,
    0x00, 0x7f, 0xff, 0x00, 0x00, 0x1f, 0xfc, 0x00,
    0x00, 0x3f, 0xfe, 0x00, 0x00, 0xff, 0xff, 0x80,
    0x01, 0xf8, 0x0f, 0xc0, 0x01, 0xe0, 0x03, 0xc0,
    0x03, 0xc0, 0x01, 0xe0, 0x03, 0xc0, 0x01, 0xe0,
    0x07, 0x80, 0x00, 0xf0, 0x07, 0x80, 0x00, 0xf0,
    0x07, 0x80, 0x00, 0xf0, 0x07, 0x80, 0x00, 0xf0,
    0x07, 0x80, 0x00, 0xf0, 0x07, 0x80, 0x00, 0xf0,
    0x07, 0xc0, 0x01, 0xf0, 0x03, 0xc0, 0x01, 0xe0,
    0x03, 0xe0, 0x03, 0xe0, 0x01, 0xf8, 0x0f, 0xc0,
    0x01, 0xff, 0xff, 0xc0, 0x00, 0xff, 0xff, 0x80,
    0x00, 0x3f, 0xfe, 0x00, 0x00, 0x0f, 0xf8, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t DIGIT_9[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00,
    0x00, 0x3f, 0xfc, 0x00, 0x00, 0x7f, 0xfe, 0x00,
    0x00, 0xff, 0xff, 0x00, 0x01, 0xf8, 0x1f, 0x80,
    0x03, 0xe0, 0x0f, 0xc0, 0x03, 0xc0, 0x07, 0xc0,
    0x03, 0xc0, 0x03, 0xe0, 0x07, 0x80, 0x03, 0xe0,
    0x07, 0x80, 0x01, 0xe0, 0x07, 0x80, 0x01, 0xe0,
    0x07, 0x80, 0x01, 0xf0, 0x07, 0x80, 0x01, 0xf0,
    0x07, 0x80, 0x01, 0xf0, 0x07, 0x80, 0x01, 0xf0,
    0x07, 0x80, 0x03, 0xf0, 0x03, 0xc0, 0x03, 0xf0,
    0x03, 0xc0, 0x07, 0xf0, 0x03, 0xe0, 0x0f, 0xf0,
    0x01, 0xf8, 0x1f, 0xf0, 0x00, 0xff, 0xfe, 0xf0,
    0x00, 0x7f, 0xfc, 0xf0, 0x00, 0x3f, 0xf8, 0xf0,
    0x00, 0x0f, 0xe0, 0xf0, 0x00, 0x00, 0x00, 0xf0,
    0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x01, 0xe0,
    0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x03, 0xe0,
    0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x07, 0xc0,
    0x00, 0x00, 0x0f, 0x80, 0x00, 0xc0, 0x3f, 0x00,
    0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xfe, 0x00,
    0x00, 0xff, 0xf8, 0x00, 0x00, 0x3f, 0xe0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t COLON[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0,
    0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x07, 0xe0,
    0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0,
    0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t SEMICOLON[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0,
    0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x07, 0xe0,
    0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0, 0x07, 0xe0,
    0x07, 0xc0, 0x0f, 0xc0, 0x0f, 0x80, 0x0f, 0x80,
    0x0f, 0x80, 0x0f, 0x00, 0x1f, 0x00, 0x1e, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t LESS_THAN_SIGN[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x78,
    0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x0f, 0xf8,
    0x00, 0x00, 0x7f, 0xf0, 0x00, 0x01, 0xff, 0x80,
    0x00, 0x0f, 0xfe, 0x00, 0x00, 0x7f, 0xf0, 0x00,
    0x01, 0xff, 0x80, 0x00, 0x0f, 0xfc, 0x00, 0x00,
    0x0f, 0xf0, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00,
    0x0f, 0xf0, 0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00,
    0x01, 0xff, 0x80, 0x00, 0x00, 0x7f, 0xf0, 0x00,
    0x00, 0x0f, 0xfe, 0x00, 0x00, 0x01, 0xff, 0x80,
    0x00, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0x0f, 0xf8,
    0x00, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x78,
    0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t EQUALS_SIGN[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf8,
    0x0f, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xf8,
    0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf8,
    0x0f, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xf8,
    0x0f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t GREATER_THAN_SIGN[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x0f, 0xe0, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00,
    0x07, 0xff, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x00,
    0x00, 0x3f, 0xf8, 0x00, 0x00, 0x07, 0xff, 0x00,
    0x00, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x1f, 0xf8,
    0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0xf8,
    0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x1f, 0xf8,
    0x00, 0x00, 0xff, 0xc0, 0x00, 0x07, 0xff, 0x00,
    0x00, 0x3f, 0xf8, 0x00, 0x00, 0xff, 0xc0, 0x00,
    0x07, 0xff, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00,
    0x0f, 0xe0, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t QUESTION_MARK[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0xfe, 0x00, 0x07, 0xff,
    0x80, 0x1f, 0xff, 0xe0, 0x3f, 0xff, 0xe0, 0x3f,
    0x03, 0xf0, 0x38, 0x00, 0xf8, 0x20, 0x00, 0x78,
    0x00, 0x00, 0x78, 0x00, 0x00, 0x78, 0x00, 0x00,
    0x78, 0x00, 0x00, 0xf8, 0x00, 0x00, 0xf0, 0x00,
    0x01, 0xf0, 0x00, 0x03, 0xe0, 0x00, 0x07, 0xe0,
    0x00, 0x0f, 0xc0, 0x00, 0x1f, 0x80, 0x00, 0x3f,
    0x00, 0x00, 0x7c, 0x00, 0x00, 0x78, 0x00, 0x00,
    0xf8, 0x00, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0xf0,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00,
    0x00, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0x00, 0xf0,
    0x00, 0x00, 0xf0, 0x00, 0x00, 0xf0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t COMMERCIAL_AT[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0xfe, 0x00, 0x00, 0x0f, 0xff, 0x80,
    0x00, 0x1f, 0xff, 0xc0, 0x00, 0x7f, 0x03, 0xe0,
    0x00, 0xfc, 0x00, 0xf0, 0x01, 0xf0, 0x00, 0x78,
    0x01, 0xe0, 0x00, 0x38, 0x03, 0xc0, 0x00, 0x3c,
    0x07, 0xc0, 0x00, 0x1c, 0x07, 0x80, 0x00, 0x1c,
    0x07, 0x80, 0x1f, 0x1c, 0x0f, 0x00, 0x7f, 0xdc,
    0x0f, 0x00, 0xff, 0xfc, 0x0f, 0x01, 0xf0, 0xfc,
    0x1e, 0x03, 0xe0, 0x7c, 0x1e, 0x03, 0xc0, 0x3c,
    0x1e, 0x07, 0xc0, 0x3c, 0x1e, 0x07, 0x80, 0x1c,
    0x1e, 0x07, 0x80, 0x1c, 0x1e, 0x07, 0x80, 0x1c,
    0x1e, 0x07, 0x80, 0x1c, 0x1e, 0x07, 0x80, 0x1c,
    0x1e, 0x07, 0x80, 0x1c, 0x1e, 0x07, 0xc0, 0x3c,
    0x1e, 0x03, 0xc0, 0x3c, 0x0f, 0x03, 0xe0, 0x7c,
    0x0f, 0x01, 0xf0, 0xfc, 0x0f, 0x00, 0xff, 0xfc,
    0x0f, 0x80, 0x7f, 0xdc, 0x07, 0x80, 0x1f, 0x1c,
    0x07, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00,
    0x03, 0xe0, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00,
    0x00, 0xf8, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00,
    0x00, 0x3f, 0x80, 0x40, 0x00, 0x0f, 0xff, 0xe0,
    0x00, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x7f, 0x80,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const __flash uint8_t DEGREE_SIGN[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x07, 0xc0, 0x1f, 0xf0, 0x3f, 0xf8,
    0x78, 0x3c, 0x70, 0x1c, 0xe0, 0x0e, 0xe0, 0x0e,
    0xe0, 0x0e, 0xe0, 0x0e, 0xe0, 0x0e, 0x70, 0x1c,
    0x78, 0x3c, 0x3f, 0xf8, 0x1f, 0xf0, 0x07, 0xc0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const __flash Glyph glyphs[] = {
    {0x0020, 24, SPACE},
    {0x0021, 16, EXCLAMATION_MARK},
    {0x0023, 32, NUMBER_SIGN},
    {0x0025, 32, PERCENT_SIGN},
    {0x002a, 24, ASTERISK},
    {0x002b, 24, PLUS_SIGN},
    {0x002c, 16, COMMA},
    {0x002d, 24, HYPHEN_MINUS},
    {0x002e, 16, FULL_STOP},
    {0x002f, 32, SOLIDUS},
    {0x0030, 32, DIGIT_0},
    {0x0031, 32, DIGIT_1},
    {0x0032, 32, DIGIT_2},
    {0x0033, 32, DIGIT_3},
    {0x0034, 32, DIGIT_4},
    {0x0035, 32, DIGIT_5},
    {0x0036, 32, DIGIT_6},
    {0x0037, 32, DIGIT_7},
    {0x0038, 32, DIGIT_8},
    {0x0039, 32, DIGIT_9},
    {0x003a, 16, COLON},
    {0x003b, 16, SEMICOLON},
    {0x003c, 32, LESS_THAN_SIGN},
    {0x003d, 32, EQUALS_SIGN},
    {0x003e, 32, GREATER_THAN_SIGN},
    {0x003f, 24, QUESTION_MARK},
    {0x0040, 32, COMMERCIAL_AT},
    {0x00b0, 16, DEGREE_SIGN}
};

const __flash Font dejaVuFont = {glyphs, array_length(glyphs), HEIGHT, SPACE_MONO1};
