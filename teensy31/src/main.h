#ifndef _MAIN_h
#define _MAIN_h

#include "cpu2.h"
#include "fft.h"

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
#define CHIN_FFT_VERT 0
#define CHIN_FFT_HORIZ 1
#define CHIN_CLONE_MID 2
#define CHIN_MIRROR_MID 3

//palettes
#define PALETTE_FIRST 0
#define PALETTE_LAST 11

//increment or decrement enum
#define INCREMENT 255
#define DECREMENT 254

#define STRIP_LEN 145

#define OUTLINE_HORIZONTAL ((11 + 9 + 11) * 2)
#define OUTLINE_VERTICAL ((23) * 2)

 
struct popstar_struct {
    fft_struct fft;
    cpu2_struct cpu2;

    int8_t menu_state = 2;
    int8_t menu_state_last = 0;

    int8_t background_mode = PATTERN_FFT_VERT_BARS_DOWN;
    int8_t background_mode_last = background_mode;

    CRGB Background_Array[MATRIX_WIDTH][MATRIX_HEIGHT];  // actual background Array

    int8_t chin_mode = CHIN_FFT_HORIZ;
    CRGB chin_array[CHIN_WIDTH][CHIN_HEIGHT];

    int  line_state_last = 0;
    int line_state = 0;

    // auto update true or false
    boolean palette_auto = true;
    boolean background_auto = true;

    CRGB outer_horiz[OUTLINE_HORIZONTAL];
    CRGB outer_vert[OUTLINE_VERTICAL];
  

    CRGBPalette16 PaletteNoiseCurrent = CRGBPalette16(PartyColors_p);
    CRGBPalette16 PaletteAniCurrent = CRGBPalette16(PartyColors_p);

    CHSV color1 = CHSV(0, 255, 255);
    CHSV color2 = CHSV(64, 255, 255);
};

#endif