#ifndef _MAIN_h
#define _MAIN_h

#include "cpu2.h"
#include "fft.h"

#define FAN_PIN 22

// main background
#define MATRIX_WIDTH 24
#define MATRIX_HEIGHT 16
#define MATRIX_CENTER_X (MATRIX_WIDTH / 2)
#define MATRIX_CENTER_Y (MATRIX_HEIGHT / 2)

#define CHIN_HEIGHT 12
#define CHIN_WIDTH 7

// background modes
#define PATTERN_FIRST 0
#define PATTERN_FFT_FIRST 0
#define PATTERN_FFT_HORZ_BARS_LEFT 0
#define PATTERN_FFT_HORZ_BARS_RIGHT 1
#define PATTERN_FFT_HORZ_BARS_STATIC 2
#define PATTERN_FFT_VERT_BARS_UP 3
#define PATTERN_FFT_VERT_BARS_DOWN 4
#define PATTERN_FFT_VERT_BARS_STATIC 5
#define PATTERN_FFT_LAST 5
#define PATTERN_NOISE_FIRST 6
#define PATTERN_NOISE_1 6
#define PATTERN_NOISE_2 7
#define PATTERN_NOISE_3 8
#define PATTERN_NOISE_4 9
#define PATTERN_NOISE_5 10
#define PATTERN_NOISE_6 11
#define PATTERN_NOISE_7 12
#define PATTERN_NOISE_8 13
#define PATTERN_NOISE_9 14
#define PATTERN_NOISE_10 15
#define PATTERN_NOISE_11 16
#define PATTERN_NOISE_LAST 16
#define PATTERN_ANI_FIRST 17
#define PATTERN_ANI_GLITTER 17
#define PATTERN_ANI_JUGGLE 18
#define PATTERN_ANI_DRIFT 19
#define PATTERN_ANI_DRIFT2 20
#define PATTERN_ANI_MUNCH 21
#define PATTERN_ANI_SNAKE 22
#define PATTERN_ANI_WAVE 23
#define PATTERN_ANI_LIFE 24
#define PATTERN_ANI_LAST 24
#define PATTERN_LAST 24
//chin
#define CHIN_FIRST 0
#define CHIN_FFT_VERT 0
#define CHIN_CLONE_MID 1
#define CHIN_FFT_HORIZ 2
#define CHIN_MIRROR_MID 3
#define CHIN_LAST 3

//palettes
#define PALETTE_FIRST 0
#define PALETTE_LAST 11

//increment or decrement enum
#define INCREMENT 255
#define DECREMENT 254

#define STRIP_LEN 145

#define OUTLINE_HORIZONTAL ((11 + 9 + 11) * 2)
#define OUTLINE_VERTICAL ((23) * 2)

#define STATE_RUNNING 0
#define STATE_STARTING 1
#define STATE_STOPPING 2
#define STATE_STOPPED 3

#define OUTLINE_FIRST 0
#define  OUTLINE_BEAT_V   0
#define OUTLINE_CONFETTI_H 1
#define OUTLINE_CONFETTI_V 2
#define OUTLINE_BEAT_H  3
#define OUTLINE_SINELON_H 4
#define OUTLINE_SINELON_V 5
#define OUTLINE_LAST 3

struct popstar_struct {
    fft_struct fft;
    cpu2_struct cpu2;
    cpu2_struct cpu2_last;

    int8_t state = STATE_STOPPED;
    int8_t state_last = STATE_STOPPED;

    int8_t pattern_mode = PATTERN_FFT_VERT_BARS_DOWN;
    int8_t pattern_mode_last = pattern_mode;

    CRGB pattern_array[MATRIX_WIDTH][MATRIX_HEIGHT];  // actual background Array

    int8_t chin_mode = CHIN_FFT_HORIZ;
    CRGB chin_array[CHIN_WIDTH][CHIN_HEIGHT];

    int line_state_last = 0;
    int line_state = 0;

    // auto update true or false
    boolean palette_auto = true;
    boolean background_auto = true;
    boolean spotlight = true;

    uint8_t outline_mode = OUTLINE_BEAT_H;

    CRGB outer_horiz[OUTLINE_HORIZONTAL];
    CRGB outline_vert[OUTLINE_VERTICAL];
    CRGB outline_vert_overlay[OUTLINE_VERTICAL];

    CRGBPalette16 PaletteNoiseCurrent = CRGBPalette16(PartyColors_p);
    CRGBPalette16 PaletteAniCurrent = CRGBPalette16(PartyColors_p);

    CHSV color1 = CHSV(0, 255, 255);
    CHSV color2 = CHSV(64, 255, 255);

    int outline_vert_mask = 0;
};

#endif