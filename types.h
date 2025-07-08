/* 
 * File:   types.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 17. September 2023, 20:33
 */

#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include "libtft/types.h"

/**
 * Temperature read from transmitter including
 * additional information.
 */
typedef struct {
    uint16_t raw;
    int8_t power;
} Temperature;

/**
 * Pointer to a function that takes no argument and returns void.
 */
typedef void (*Function)(void);

#endif /* TYPES_H */

