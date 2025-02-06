/* 
 * File:   utils.h
 * Author: torsten.roemer@luniks.net
 *
 * Created on 07. February 2025, 00:25
 */

#ifndef UTILS_H
#define UTILS_H

/**
 * Computes the length of the given array.
 */
#define array_length(array) (sizeof(array) / sizeof(array[0]))

/**
 * Returns the greater of both given numbers.
 * https://stackoverflow.com/questions/3437404/min-and-max-in-c
 */
#define max(a, b)            \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

/**
 * Returns the smaller of both given numbers.
 * https://stackoverflow.com/questions/3437404/min-and-max-in-c
 */
#define min(a, b)            \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})


#endif /* UTILS_H */

