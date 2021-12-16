#include <Arduino.h>
#include "main.h"
#include "FastLED.h"

static void FillNoise16(void);
static void FillNoise8(void);

// static uint16_t speed = 0; // speed is set dynamically once we've started up
static uint16_t scale = 2000;  // scale is set dynamically once we've started up
static uint8_t noise[MATRIX_WIDTH][MATRIX_WIDTH];
static uint16_t x_noise;
static uint16_t y_noise;
static uint16_t z_noise;

static void Caleidoscope1(CRGB Background_Array[MATRIX_WIDTH][MATRIX_HEIGHT]) {
    for (int x = 0; x < MATRIX_WIDTH / 2; x++) {
        for (int y = 0; y < MATRIX_HEIGHT / 2; y++) {
            Background_Array[MATRIX_WIDTH - 1 - x][y] = Background_Array[y][x];
            Background_Array[MATRIX_WIDTH - 1 - x][MATRIX_HEIGHT - 1 - y] = Background_Array[x][y];
            Background_Array[x][MATRIX_HEIGHT - 1 - y] = Background_Array[y][x];
        }
    }
}

static void Caleidoscope2(CRGB Background_Array[MATRIX_WIDTH][MATRIX_HEIGHT]) {
    for (int x = 0; x < MATRIX_WIDTH / 2; x++) {
        for (int y = 0; y < MATRIX_HEIGHT / 2; y++) {
            Background_Array[MATRIX_WIDTH - 1 - x][y] = Background_Array[x][y];
            Background_Array[x][MATRIX_HEIGHT - 1 - y] = Background_Array[x][y];
            Background_Array[MATRIX_WIDTH - 1 - x][MATRIX_HEIGHT - 1 - y] = Background_Array[x][y];
        }
    }
}

static void mapNoiseToLEDsUsingPalette2(const float band[], CRGB Background_Array[MATRIX_WIDTH][MATRIX_HEIGHT], const struct CRGBPalette16 *palette) {
    static uint8_t ihue = 0;

    for (int i = 0; i < MATRIX_WIDTH; i++) {
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
            // We use the value at the (i,j) coordinate in the noise
            // array for our brightness, and the flipped value from (j,i)
            // for our pixel's index into the color palette.

            uint8_t index = (noise[j][i] / 2) + (band[1] / 2 + (band[6] / 4)) * 160;
            uint8_t bri = (noise[i][j] / 2) + (band[1] / 2 + (band[6] / 4)) * 160;

            // if this palette is a 'loop', add a slowly-changing base value
            //	if (colorLoop) {
            //		index += ihue;
            //	}

            // brighten up, as the color palette itself often contains the
            // light/dark dynamic range desired
            if (bri > 200) {
                bri = 255;
            } else {
                bri = dim8_raw(bri * 3);
            }

            CRGB color = ColorFromPalette(*palette, index, bri);
            Background_Array[i][j] = color;
        }
    }

    ihue += 1;
}

void pattern_noise(CRGB Background_Array[MATRIX_WIDTH][MATRIX_HEIGHT], const struct popstar_struct *helmet) {
    switch (helmet->background_mode) {
        case BACKGROUND_NOISE_1:
            scale = 55;
            x_noise = x_noise + helmet->fft.band[1] * 5;  // 0 to 5
            y_noise = y_noise + helmet->fft.band[1] * 10;
            if (helmet->fft.band[6] > 0.02) z_noise = z_noise + 2;
            if (helmet->fft.band[9] > 0.02) z_noise = z_noise + 2;
            // else{ z=z-2;}
            FillNoise16();
            break;
        case BACKGROUND_NOISE_2:
            scale = 40;
            x_noise = x_noise + helmet->fft.band[1] * 10;
            // y = y ;
            // if (band[4||5||6] > 0.06) z=z+2;
            // if (band[7||8||9] > 0.06) z=z+1;
            // else{ z=z-2;}
            FillNoise8();
            break;
        case BACKGROUND_NOISE_3:
            scale = 50;
            x_noise = x_noise + helmet->fft.band[1] * 10;
            // y = y ;
            // if (band[4||5||6] > 0.06) z=z+2;
            // if (band[7||8||9] > 0.06) z=z+1;
            // else{ z=z-2;}
            FillNoise8();
            break;
        case BACKGROUND_NOISE_4:
            scale = 40;
            x_noise = x_noise + helmet->fft.band[1] * 12;
            // y = y + band[1]*40;
            if (helmet->fft.band[4 || 5 || 6] > 0.04) z_noise = z_noise + 2;
            if (helmet->fft.band[7 || 8 || 9] > 0.04) z_noise = z_noise + 1;
            // else{ z=z-2;}
            FillNoise8();
            break;
        case BACKGROUND_NOISE_5:
            scale = 60;
            x_noise = x_noise + helmet->fft.band[0] * 10;
            y_noise = y_noise + helmet->fft.band[0] * 5;
            // if (band[4||5||6] > 0.06) z=z+1;
            // if (band[7||8||9] > 0.06) z=z+1;
            z_noise = z_noise - 2;
            FillNoise16();
            break;
        case BACKGROUND_NOISE_6:
            scale = 140 - (helmet->fft.band[1] * 100);
            x_noise = x_noise + helmet->fft.band[1] * 20;
            y_noise = y_noise + helmet->fft.band[1] * 5;
            // if (band[4||5||6] > 0.06) z=z+3;
            // if (band[7||8||9] > 0.06) z=z+1;
            // else{ z=z-2;}
            FillNoise8();
            break;
        case BACKGROUND_NOISE_7:
            scale = 40;
            x_noise = x_noise - 1;
            y_noise = y_noise + helmet->fft.band[0] * 20;
            if (helmet->fft.band[4 || 5 || 6] > 0.06) z_noise = z_noise + 3;
            if (helmet->fft.band[7 || 8 || 9] > 0.06) z_noise = z_noise + 3;
            z_noise = z_noise - 1;
            FillNoise8();
            break;
        case BACKGROUND_NOISE_8:
            scale = 100 - (helmet->fft.band[1] * 100);
            x_noise = x_noise + helmet->fft.band[1] * 5;
            y_noise = y_noise + helmet->fft.band[1] * 10;
            if (helmet->fft.band[4 || 5 || 6] > 0.06) z_noise = z_noise + 3;
            if (helmet->fft.band[7 || 8 || 9] > 0.06)
                z_noise = z_noise + 1;
            else {
                z_noise = z_noise - 2;
            }
            FillNoise16();
            break;
        case BACKGROUND_NOISE_9:
            x_noise = x_noise + helmet->fft.band[1] * 20;
            y_noise = y_noise + helmet->fft.band[1] * 10;
            scale = 60;
            // if (band[4||5||6] > 0.06) z=z-1;
            // if (band[7||8||9] > 0.06) z=z-1;
            z_noise = z_noise - 2;
            FillNoise8();
            break;
        case BACKGROUND_NOISE_10:
            x_noise = x_noise + helmet->fft.band[1] * 20;
            y_noise = y_noise + helmet->fft.band[1] * 20;
            scale = 200;
            if (helmet->fft.band[4 || 5 || 6] > 0.06) z_noise = z_noise - 1;
            if (helmet->fft.band[7 || 8 || 9] > 0.06)
                z_noise = z_noise - 1;
            else {
                z_noise = z_noise - 2;
            }
            FillNoise16();
            break;
        case BACKGROUND_NOISE_11:
            x_noise = x_noise + helmet->fft.band[1] * 80;
            y_noise = y_noise + helmet->fft.band[1] * 80;
            scale = 60;
            if (helmet->fft.band[4 || 5 || 6] > 0.06) z_noise = z_noise - 1;
            if (helmet->fft.band[7 || 8 || 9] > 0.06)
                z_noise = z_noise - 1;
            else {
                z_noise = z_noise - 2;
            }
            FillNoise8();
            break;
    }

    mapNoiseToLEDsUsingPalette2(helmet->fft.band, Background_Array,&(helmet->PaletteNoiseCurrent));

    switch (helmet->background_mode) {
        case BACKGROUND_NOISE_1:
        case BACKGROUND_NOISE_2:
        case BACKGROUND_NOISE_5:
        case BACKGROUND_NOISE_6:
        case BACKGROUND_NOISE_7:
        case BACKGROUND_NOISE_9:
        case BACKGROUND_NOISE_11:
            Caleidoscope2(Background_Array);
            break;
        case BACKGROUND_NOISE_3:
        case BACKGROUND_NOISE_4:
        case BACKGROUND_NOISE_8:
        case BACKGROUND_NOISE_10:
            Caleidoscope1(Background_Array);
            break;
    }
}

static void FillNoise16(void) {
    for (int i = 0; i < MATRIX_WIDTH; i++) {
        int ioffset = scale * i;
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
            int joffset = scale * j;
            noise[i][j] = inoise16(x_noise + ioffset, y_noise + joffset);
        }
    }
}

static void FillNoise8(void) {
    for (int i = 0; i < MATRIX_WIDTH; i++) {
        int ioffset = scale * i;
        for (int j = 0; j < MATRIX_HEIGHT; j++) {
            int joffset = scale * j;
            noise[i][j] = inoise8(x_noise + ioffset, y_noise + joffset);
        }
    }
}
