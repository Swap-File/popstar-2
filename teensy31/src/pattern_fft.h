#include <Arduino.h>
#include "main.h"
#include "FastLED.h"

static CHSV map_hsv(uint8_t input, uint8_t in_min, uint8_t in_max, const struct CHSV *out_starting, const struct CHSV *out_ending) {
    if (input <= in_min) return CHSV(*out_starting);
    if (input >= in_max) return CHSV(*out_ending);

    // calculate shortest path between colors
    int16_t shortest_path = out_ending->h;  // no rollover
    if ((((int16_t)out_ending->h) + 256) - ((int16_t)out_starting->h) <= 127) {
        shortest_path += 256;  // rollover
    } else if ((int16_t)(out_starting->h) - (((int16_t)out_ending->h) - 255) <= 127) {
        shortest_path -= 256;  // rollunder
    }

    return CHSV(((input - in_min) * (shortest_path - out_starting->h + 1) / (in_max - in_min + 1) + out_starting->h),
                (input - in_min) * (out_ending->s - out_starting->s + 1) / (in_max - in_min + 1) + out_starting->s,
                (input - in_min) * (out_ending->v - out_starting->v + 1) / (in_max - in_min + 1) + out_starting->v);
}

static void calcfftcolor(CHSV *temp_color, uint8_t input, const struct CHSV *color1, const struct CHSV *color2) {
    // make the tip of the color be color 2
    *temp_color = (input > 240) ? map_hsv(input, 240, 255, color1, color2) : *color1;

    // ignore brightness, max it.
    temp_color->v = input;

    return;
}

void pattern_fft(CRGB Background_Array[MATRIX_WIDTH][MATRIX_HEIGHT], const struct popstar_struct *helmet) {
    if (helmet->background_mode == BACKGROUND_FFT_HORZ_BARS_RIGHT) {
        // move  data left 1
        for (uint8_t x = 1; x < 24; x++) {
            for (uint8_t y = 0; y < 16; y++) {
                Background_Array[x - 1][y] = Background_Array[x][y];
            }
        }
    } else if (helmet->background_mode == BACKGROUND_FFT_HORZ_BARS_LEFT) {
        // move  data right 1
        for (uint8_t x = 23; x > 0; x--) {
            for (uint8_t y = 0; y < 16; y++) {
                Background_Array[x][y] = Background_Array[x - 1][y];
            }
        }
    } else if (helmet->background_mode == BACKGROUND_FFT_VERT_BARS_UP) {
        // move  data up  1
        for (uint8_t y = 15; y > 0; y--) {
            for (uint8_t x = 0; x < 24; x++) {
                Background_Array[x][y] = Background_Array[x][y - 1];
            }
        }
    } else if (helmet->background_mode == BACKGROUND_FFT_VERT_BARS_DOWN) {
        // move data up  1
        for (uint8_t y = 1; y < 16; y++) {
            for (uint8_t x = 0; x < 24; x++) {
                Background_Array[x][y - 1] = Background_Array[x][y];
            }
        }
    }

    if (helmet->background_mode == BACKGROUND_FFT_HORZ_BARS_RIGHT ||
        helmet->background_mode == BACKGROUND_FFT_HORZ_BARS_LEFT) {
        for (uint8_t i = 0; i < 16; i++) {
            // make the tip of the color be color 2
            CHSV temp_color;
            calcfftcolor(&temp_color, helmet->fft.value_16[i], &(helmet->color1), &(helmet->color2));
            if (helmet->background_mode == BACKGROUND_FFT_HORZ_BARS_RIGHT)
                Background_Array[23][i] = temp_color;
            else if (helmet->background_mode == BACKGROUND_FFT_HORZ_BARS_LEFT)
                Background_Array[0][i] = temp_color;
        }
    } else if (helmet->background_mode == BACKGROUND_FFT_HORZ_BARS_STATIC) {
        for (uint8_t i = 0; i < 16; i++) {
            // make the tip of the color be color 2
            CHSV temp_color;
            calcfftcolor(&temp_color, helmet->fft.value_16[i], &(helmet->color1), &(helmet->color2));
            for (uint8_t index = 0; index < 24; index++) {
                Background_Array[index][i] = temp_color;
            }
        }
    } else if (helmet->background_mode == BACKGROUND_FFT_VERT_BARS_UP ||
               helmet->background_mode == BACKGROUND_FFT_VERT_BARS_DOWN) {
        for (uint8_t i = 0; i < 16; i++) {
            // make the tip of the color be color 2
            CHSV temp_color;
            calcfftcolor(&temp_color, helmet->fft.value_16[i], &(helmet->color1), &(helmet->color2));
            if (helmet->background_mode == BACKGROUND_FFT_VERT_BARS_UP) {
                Background_Array[i][0] = temp_color;
                Background_Array[23 - i][0] = temp_color;
            } else if (helmet->background_mode == BACKGROUND_FFT_VERT_BARS_DOWN) {
                Background_Array[i][15] = temp_color;
                Background_Array[23 - i][15] = temp_color;
            }
        }
    } else if (helmet->background_mode == BACKGROUND_FFT_VERT_BARS_STATIC) {
        for (uint8_t i = 0; i < 12; i++) {
            // make the tip of the color be color 2
            CHSV temp_color;
            calcfftcolor(&temp_color, helmet->fft.value_12[i], &(helmet->color1), &(helmet->color2));
            for (uint8_t index = 0; index < 16; index++) {
                Background_Array[i][index] = temp_color;
                Background_Array[23 - i][index] = temp_color;
            }
        }
    }
}
