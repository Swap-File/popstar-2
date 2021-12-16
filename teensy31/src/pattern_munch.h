#include <Arduino.h>
#include "main.h"
#include "FastLED.h"

static byte count = 0;
static byte dir = 1;
static byte flip = 0;
static byte generation = 0;

void pattern_munch_draw(CRGB Background_Array[MATRIX_WIDTH][MATRIX_HEIGHT], const struct CRGBPalette16 *palette) {
    for (byte x = 0; x < MATRIX_WIDTH; x++) {
        for (byte y = 0; y < MATRIX_HEIGHT; y++) {
            Background_Array[x][y] = (x ^ y ^ flip) < count ? ColorFromPalette(*palette,((x ^ y) << 3) + generation) : CRGB::Black;
        }
    }

    count += dir;

    if (count <= 0 || count >= MATRIX_WIDTH) {
        dir = -dir;
    }

    if (count <= 0) {
        if (flip == 0)
            flip = 31;
        else
            flip = 0;
    }

    generation++;
}
