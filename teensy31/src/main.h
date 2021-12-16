#ifndef _MAIN_h
#define _MAIN_h

#include "fft.h"

// main background
#define MATRIX_WIDTH 24
#define MATRIX_HEIGHT 16
#define MATRIX_CENTER_X (MATRIX_WIDTH / 2)
#define MATRIX_CENTER_Y (MATRIX_HEIGHT / 2)

// background modes
#define BACKGROUND_FIRST 0
#define BACKGROUND_FFT_FIRST 0
#define BACKGROUND_FFT_HORZ_BARS_LEFT 0
#define BACKGROUND_FFT_HORZ_BARS_RIGHT 1
#define BACKGROUND_FFT_HORZ_BARS_STATIC 2
#define BACKGROUND_FFT_VERT_BARS_UP 3
#define BACKGROUND_FFT_VERT_BARS_DOWN 4
#define BACKGROUND_FFT_VERT_BARS_STATIC 5
#define BACKGROUND_FFT_LAST 5
#define BACKGROUND_NOISE_FIRST 6
#define BACKGROUND_NOISE_1 6
#define BACKGROUND_NOISE_2 7
#define BACKGROUND_NOISE_3 8
#define BACKGROUND_NOISE_4 9
#define BACKGROUND_NOISE_5 10
#define BACKGROUND_NOISE_6 11
#define BACKGROUND_NOISE_7 12
#define BACKGROUND_NOISE_8 13
#define BACKGROUND_NOISE_9 14
#define BACKGROUND_NOISE_10 15
#define BACKGROUND_NOISE_11 16
#define BACKGROUND_NOISE_LAST 16
#define BACKGROUND_ANI_FIRST 17
#define BACKGROUND_ANI_GLITTER 17
#define BACKGROUND_ANI_JUGGLE 18
#define BACKGROUND_ANI_DRIFT 19
#define BACKGROUND_ANI_DRIFT2 20
#define BACKGROUND_ANI_MUNCH 21
#define BACKGROUND_ANI_SNAKE 22
#define BACKGROUND_ANI_WAVE 23
#define BACKGROUND_ANI_LIFE 24
#define BACKGROUND_ANI_LAST 24
#define BACKGROUND_LAST 24

//palettes
#define PALETTE_FIRST 0
#define PALETTE_LAST 11

//increment or decrement enum
#define INCREMENT 255
#define DECREMENT 254


struct popstar_struct {
    fft_struct fft;

    int8_t menu_state = 0;
    int8_t menu_state_last = 0;

    int8_t background_mode = BACKGROUND_ANI_GLITTER;
    int8_t background_mode_last = background_mode;

    CRGB Background_Array[MATRIX_WIDTH][MATRIX_HEIGHT];  // actual background Array
    CRGB Snapshot_Array[MATRIX_WIDTH][MATRIX_HEIGHT];    // snapshot for fades

    // auto update true or false
    boolean palette_auto = true;
    boolean background_auto = true;

    CRGBPalette16 PaletteNoiseCurrent = CRGBPalette16(PartyColors_p);
    CRGBPalette16 PaletteAniCurrent = CRGBPalette16(PartyColors_p);
    CRGBPalette16 PaletteNoiseTarget = CRGBPalette16(PartyColors_p);
    CRGBPalette16 PaletteAniTarget = CRGBPalette16(PartyColors_p);

    CHSV color1 = CHSV(0, 255, 255);
    CHSV color2 = CHSV(64, 255, 255);
    CHSV color1Target = CHSV(0, 255, 255);
    CHSV color2Target = CHSV(64, 255, 255);

};

#endif