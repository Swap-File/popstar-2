#include <Arduino.h>

#include "FastLED.h"
#include "Metro.h"
#include "main.h"

Metro PaletteBlender = Metro(10, 1);
#define BLEND_AMT 3

static int8_t requested_palette = 0;
static int32_t palette_change_time = 0;
static int32_t background_change_time = 0;
static uint32_t ani_timer_delay = 5;
static uint32_t ani_timer = 0;

#include "pattern_fft.h"
#include "pattern_life.h"
#include "pattern_minor.h"
#include "pattern_munch.h"
#include "pattern_noise.h"
#include "pattern_snake.h"

static CRGBPalette16 PaletteNoiseTarget = CRGBPalette16(PartyColors_p);
static CRGBPalette16 PaletteAniTarget = CRGBPalette16(PartyColors_p);

static CHSV color1Target = CHSV(0, 255, 255);
static CHSV color2Target = CHSV(64, 255, 255);

void pattern_update(const struct popstar_struct *helmet, CRGB Background_Array[MATRIX_WIDTH][MATRIX_HEIGHT], bool fresh_fft, CRGB Chin_array[CHIN_WIDTH][CHIN_HEIGHT]) {
    if (fresh_fft)
        pattern_fft_chin(Chin_array, helmet);

    if (helmet->pattern_mode <= PATTERN_FFT_LAST && helmet->pattern_mode >= PATTERN_FFT_FIRST && fresh_fft)
        pattern_fft(Background_Array, helmet);
    if (helmet->pattern_mode <= PATTERN_NOISE_LAST && helmet->pattern_mode >= PATTERN_NOISE_FIRST && fresh_fft)
        pattern_noise(Background_Array, helmet);

    if (helmet->pattern_mode <= PATTERN_ANI_LAST && helmet->pattern_mode >= PATTERN_ANI_FIRST) {
        if (helmet->pattern_mode != helmet->pattern_mode_last || millis() - ani_timer > ani_timer_delay) {
            switch (helmet->pattern_mode) {
                case PATTERN_ANI_MUNCH:
                    pattern_munch_draw(Background_Array, &(helmet->PaletteAniCurrent));
                    ani_timer_delay = 15;
                    break;
                case PATTERN_ANI_DRIFT:
                    pattern_minor_drift1(Background_Array, &(helmet->PaletteAniCurrent));
                    ani_timer_delay = 10;
                    break;
                case PATTERN_ANI_GLITTER:
                    if (helmet->pattern_mode_last != helmet->pattern_mode)
                        pattern_minor_glitter(3, Background_Array, &(helmet->PaletteAniCurrent));
                    pattern_minor_glitter(1, Background_Array, &(helmet->PaletteAniCurrent));
                    ani_timer_delay = 10;
                    break;
                case PATTERN_ANI_JUGGLE:
                    pattern_minor_juggle(Background_Array);
                    ani_timer_delay = 10;
                    break;
                case PATTERN_ANI_SNAKE:
                    if (helmet->pattern_mode_last != helmet->pattern_mode)
                        pattern_snake_setup();
                    pattern_snake_draw(Background_Array, &(helmet->PaletteAniCurrent));
                    ani_timer_delay = 20;
                    break;
                case PATTERN_ANI_DRIFT2:
                    pattern_minor_drift2(Background_Array, &(helmet->PaletteAniCurrent));
                    ani_timer_delay = 15;
                    break;
                case PATTERN_ANI_WAVE:
                    pattern_minor_wave(Background_Array, &(helmet->PaletteAniCurrent));
                    ani_timer_delay = 15;
                    break;
                case PATTERN_ANI_LIFE:
                    if (helmet->pattern_mode_last != helmet->pattern_mode)
                        pattern_life_setup();
                    pattern_life_draw(Background_Array, &(helmet->PaletteAniCurrent));
                    ani_timer_delay = 15;
                    break;
            }
            ani_timer = millis();
        }
    }

    if (helmet->chin_mode == CHIN_CLONE_MID || helmet->chin_mode == CHIN_MIRROR_MID) { //temp do both
        for (uint8_t y = 0; y < CHIN_HEIGHT; y++) {
            for (uint8_t x = 0; x < CHIN_WIDTH; x++) {
                Chin_array[x][y] = Background_Array[x + 9][y];
            }
        }
    }
}

void pattern_background_timer(void) {
    background_change_time = millis();
}

void pattern_set_background(uint8_t number, struct popstar_struct *helmet) {
    if (number != INCREMENT && number != DECREMENT) {
        helmet->pattern_mode = number;
        number = 0;
    }

    if (number == INCREMENT) number = 1;
    if (number == DECREMENT) number = -1;

    if (helmet->pattern_mode >= PATTERN_NOISE_FIRST && helmet->pattern_mode <= PATTERN_NOISE_LAST) {
        helmet->pattern_mode += number;
        if (helmet->pattern_mode < PATTERN_NOISE_FIRST)
            helmet->pattern_mode = PATTERN_NOISE_LAST;
        if (helmet->pattern_mode > PATTERN_NOISE_LAST)
            helmet->pattern_mode = PATTERN_NOISE_FIRST;
    }

    if (helmet->pattern_mode >= PATTERN_ANI_FIRST && helmet->pattern_mode <= PATTERN_ANI_LAST) {
        helmet->pattern_mode += number;
        if (helmet->pattern_mode < PATTERN_ANI_FIRST)
            helmet->pattern_mode = PATTERN_ANI_LAST;
        if (helmet->pattern_mode > PATTERN_ANI_LAST)
            helmet->pattern_mode = PATTERN_ANI_FIRST;
    }

    if (helmet->pattern_mode >= PATTERN_FFT_FIRST && helmet->pattern_mode <= PATTERN_FFT_LAST) {
        helmet->pattern_mode = random8(PATTERN_NOISE_FIRST, PATTERN_NOISE_LAST + 1);  // exit to random sound mode
    }

    background_change_time = millis();
}


void pattern_set_palette(uint8_t req, bool immediate, struct popstar_struct *helmet) {
    if (req == INCREMENT)
        requested_palette++;
    else if (req == INCREMENT)
        requested_palette--;
    else
        requested_palette = req;

    requested_palette = (requested_palette + 12) % 12;  // wrap palettes

    CHSV r_hsv = CHSV(HUE_RED, 255, 255);
    CRGB r = r_hsv;
    CHSV o_hsv = CHSV(HUE_ORANGE, 255, 255);
    CRGB o = o_hsv;
    CHSV y_hsv = CHSV(HUE_YELLOW, 255, 255);
    CRGB y = y_hsv;
    CHSV g_hsv = CHSV(HUE_GREEN, 255, 255);
    CRGB g = g_hsv;
    CHSV a_hsv = CHSV(HUE_AQUA, 255, 255);
    CRGB a = a_hsv;
    CHSV b_hsv = CHSV(HUE_BLUE, 255, 255);
    CRGB b = b_hsv;
    CHSV p_hsv = CHSV(HUE_PURPLE, 255, 255);
    CRGB p = p_hsv;
    CHSV pi_hsv = CHSV(HUE_PINK, 255, 255);
    CRGB pi = pi_hsv;
    CRGB bl = CRGB::Black;
    CHSV w_hsv = CHSV(0, 255, 255);
    CRGB w = CRGB::White;
    CHSV ra_hsv = CHSV(75, 255, 255);
    CRGB ra = ra_hsv;

    switch (requested_palette) {
        case 0:
            PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, w, w, b, b, b, b, b, bl);
            PaletteAniTarget = CRGBPalette16(w, w, w, b, b, b, b, b, w, w, w, b, b, b, b, b);
            color1Target = b_hsv;
            color2Target = w_hsv;

            break;
        case 1:
            PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, w, a, g, g, g, r, bl, bl);
            PaletteAniTarget = CRGBPalette16(w, a, g, g, r, w, a, g, g, g, r, w, a, g, g, r);
            color1Target = g_hsv;
            color2Target = a_hsv;

            break;
        case 2:
            PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, a, p, pi, pi, pi, r, bl, bl);
            PaletteAniTarget = CRGBPalette16(a, p, pi, pi, r, a, p, pi, pi, pi, r, a, p, pi, pi, r);
            color1Target = pi_hsv;
            color2Target = a_hsv;

            break;
        case 3:
            PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, g, a, b, b, b, b, bl, bl);
            PaletteAniTarget = CRGBPalette16(g, a, b, b, b, g, a, b, b, b, g, a, b, b, b, b);
            color1Target = b_hsv;
            color2Target = g_hsv;

            break;
        case 4:
            PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, p, p, y, y, bl, bl, bl, bl);
            PaletteAniTarget = CRGBPalette16(p, p, y, y, p, p, y, y, p, p, y, y, p, p, y, y);
            color1Target = p_hsv;
            color2Target = y_hsv;

            break;
        case 5:
            PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, a, a, y, y, y, y, bl, bl);
            PaletteAniTarget = CRGBPalette16(a, a, y, y, y, a, a, y, y, y, y, a, a, y, y, y);
            color1Target = a_hsv;
            color2Target = y_hsv;

            break;
        case 6:
            PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, b, r, bl, bl, bl, bl, bl, bl);
            PaletteAniTarget = CRGBPalette16(b, r, b, r, b, r, b, r, b, r, b, r, b, r, b, r);
            color1Target = r_hsv;
            color2Target = b_hsv;

            break;
        case 7:
            PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, o, o, p, bl, bl, bl, bl, bl);
            PaletteAniTarget = CRGBPalette16(o, o, p, o, o, p, o, o, p, o, o, p, o, o, p, o);
            color1Target = o_hsv;
            color2Target = p_hsv;

            break;
        case 8:
            PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, r, o, y, g, bl, bl, bl, bl);
            PaletteAniTarget = CRGBPalette16(r, o, y, g, r, o, y, g, r, o, y, g, r, o, y, g);
            color1Target = r_hsv;
            color2Target = y_hsv;

            break;
        case 9:
            PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, b, b, a, ra, ra, bl, bl, bl);
            PaletteAniTarget = CRGBPalette16(b, b, a, ra, ra, b, b, a, ra, ra, b, b, a, ra, ra, b);
            color1Target = b_hsv;
            color2Target = ra_hsv;

            break;
        case 10: {
            uint8_t temp1 = random8();
            uint8_t temp2 = random8();
            PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, r, r, temp1, temp2, bl, bl, bl, bl);
            PaletteAniTarget = CRGBPalette16(r, r, temp1, temp2, r, r, temp1, temp2, r, r, temp1, temp2, r, r, temp1, temp2);
            color1Target = r_hsv;
            color2Target = CHSV(temp1, 255, 255);
        } break;
        case 11:
            PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, b, p, pi, r, bl, bl, bl, bl);
            PaletteAniTarget = CRGBPalette16(b, p, pi, r, b, p, pi, r, b, p, pi, r, b, p, pi, r);
            color1Target = b_hsv;
            color2Target = r_hsv;

            break;
    }

    if (immediate) {
        helmet->color1 = color1Target;
        helmet->color2 = color2Target;
        nblendPaletteTowardPalette(helmet->PaletteNoiseCurrent, PaletteNoiseTarget, 255);
        nblendPaletteTowardPalette(helmet->PaletteAniCurrent, PaletteAniTarget, 255);
    }

    palette_change_time = millis();
}
void pattern_auto_update(struct popstar_struct *helmet) {
    // dont advance timers when off
    if (helmet->state != STATE_RUNNING) {
        palette_change_time = millis();
        background_change_time = millis();
    }

    // 10 second palette changer
    if (millis() - palette_change_time > 10000 && helmet->palette_auto) {
        pattern_set_palette(INCREMENT, false, helmet);
        palette_change_time = millis();
    }

    // 20 second background changer
    if (millis() - background_change_time > 20000 && helmet->background_auto) {
        pattern_set_background(INCREMENT, helmet);
        background_change_time = millis();
    }

#define HEAD_ANGLE 25
    // head direction changes
    // if (helmet->pattern_mode >= PATTERN_FFT_FIRST && helmet->pattern_mode <= PATTERN_FFT_LAST || helmet->pattern_mode >= PATTERN_NOISE_FIRST && helmet->pattern_mode <= PATTERN_NOISE_LAST) {
    if (helmet->cpu2.head_roll < -HEAD_ANGLE) {
        helmet->pattern_mode = PATTERN_FFT_HORZ_BARS_RIGHT;
        background_change_time = millis();
    } else if (helmet->cpu2.head_roll > HEAD_ANGLE) {
        helmet->pattern_mode = PATTERN_FFT_HORZ_BARS_LEFT;
        background_change_time = millis();
    } else if (helmet->cpu2.head_pitch < -HEAD_ANGLE) {
        helmet->pattern_mode = PATTERN_FFT_VERT_BARS_UP;
        background_change_time = millis();
    } else if (helmet->cpu2.head_pitch > HEAD_ANGLE) {
        helmet->pattern_mode = PATTERN_FFT_VERT_BARS_DOWN;
        background_change_time = millis();
    }

    // }

    if (PaletteBlender.check()) {
        nblendPaletteTowardPalette(helmet->PaletteNoiseCurrent, PaletteNoiseTarget, BLEND_AMT);
        nblendPaletteTowardPalette(helmet->PaletteAniCurrent, PaletteAniTarget, BLEND_AMT);
        nblend(helmet->color1, color1Target, BLEND_AMT);
        nblend(helmet->color2, color2Target, BLEND_AMT);
    }
}