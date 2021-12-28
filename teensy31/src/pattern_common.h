#ifndef _PATTERN_COMMON_h
#define _PATTERN_COMMON_h

#include <Arduino.h>
#include "main.h"
#include "FastLED.h"

uint8_t pattern_common_beatcos8(accum88 beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255, uint32_t timebase = 0, uint8_t phase_offset = 0) {
    uint8_t beat = beat8(beats_per_minute, timebase);
    uint8_t beatcos = cos8(beat + phase_offset);
    uint8_t rangewidth = highest - lowest;
    uint8_t scaledbeat = scale8(beatcos, rangewidth);
    uint8_t result = lowest + scaledbeat;
    return result;
}

void pattern_common_dimall(byte value, CRGB Background_Array[MATRIX_WIDTH][MATRIX_HEIGHT]) {
    for (uint8_t y = 0; y < MATRIX_HEIGHT; y++) {
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            Background_Array[x][y].nscale8(value);
        }
    }
}

#endif