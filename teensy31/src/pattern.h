#include <Arduino.h>
#include "main.h"
#include "FastLED.h"

static int8_t requested_palette = 0;

#include "pattern_minor.h"
#include "pattern_life.h"
#include "pattern_munch.h"
#include "pattern_noise.h"
#include "pattern_snake.h"
#include "pattern_fft.h"

static int32_t palette_change_time = 0;
static int32_t background_change_time = 0;
static uint32_t ani_timer_delay = 5;
static uint32_t ani_timer = 0;

void pattern_update(const struct popstar_struct *helmet, CRGB Background_Array[MATRIX_WIDTH][MATRIX_HEIGHT]) {
    if (helmet->background_mode <= BACKGROUND_FFT_LAST && helmet->background_mode >= BACKGROUND_FFT_FIRST)
        pattern_fft(Background_Array, helmet);
    if (helmet->background_mode <= BACKGROUND_NOISE_LAST && helmet->background_mode >= BACKGROUND_NOISE_FIRST)
        pattern_noise(Background_Array, helmet);

    if (helmet->background_mode <= BACKGROUND_ANI_LAST && helmet->background_mode >= BACKGROUND_ANI_FIRST) {
        if (helmet->background_mode != helmet->background_mode_last || millis() - ani_timer > ani_timer_delay) {
            switch (helmet->background_mode) {
                case BACKGROUND_ANI_MUNCH:
                    pattern_munch_draw(Background_Array, &(helmet->PaletteAniCurrent));
                    ani_timer_delay = 15;
                    break;
                case BACKGROUND_ANI_DRIFT:
                    pattern_minor_drift1(Background_Array, &(helmet->PaletteAniCurrent));
                    ani_timer_delay = 10;
                    break;
                case BACKGROUND_ANI_GLITTER:
                    if (helmet->background_mode_last != helmet->background_mode)
                        pattern_minor_glitter(3, Background_Array, &(helmet->PaletteAniCurrent));
                    pattern_minor_glitter(1, Background_Array, &(helmet->PaletteAniCurrent));
                    ani_timer_delay = 10;
                    break;
                case BACKGROUND_ANI_JUGGLE:
                    pattern_minor_juggle(Background_Array);
                    ani_timer_delay = 10;
                    break;
                case BACKGROUND_ANI_SNAKE:
                    if (helmet->background_mode_last != helmet->background_mode)
                        pattern_snake_setup();
                    pattern_snake_draw(Background_Array, &(helmet->PaletteAniCurrent));
                    ani_timer_delay = 20;
                    break;
                case BACKGROUND_ANI_DRIFT2:
                    pattern_minor_drift2(Background_Array, &(helmet->PaletteAniCurrent));
                    ani_timer_delay = 15;
                    break;
                case BACKGROUND_ANI_WAVE:
                    pattern_minor_wave(Background_Array, &(helmet->PaletteAniCurrent));
                    ani_timer_delay = 15;
                    break;
                case BACKGROUND_ANI_LIFE:
                    if (helmet->background_mode_last != helmet->background_mode)
                        pattern_life_setup();
                    pattern_life_draw(Background_Array, &(helmet->PaletteAniCurrent));
                    ani_timer_delay = 15;
                    break;
            }
            ani_timer = millis();
        }
    }
}

void pattern_set_background(uint8_t number, struct popstar_struct *helmet) {
    if (number != INCREMENT && number != DECREMENT) {
        helmet->background_mode = number;
        number = 0;
    }

    if (number == INCREMENT) number = 1;
    if (number == DECREMENT) number = -1;

    if (helmet->background_mode >= BACKGROUND_NOISE_FIRST && helmet->background_mode <= BACKGROUND_NOISE_LAST) {
        helmet->background_mode += number;
        if (helmet->background_mode < BACKGROUND_NOISE_FIRST)
            helmet->background_mode = BACKGROUND_NOISE_LAST;
        if (helmet->background_mode > BACKGROUND_NOISE_LAST)
            helmet->background_mode = BACKGROUND_NOISE_FIRST;
    }

    if (helmet->background_mode >= BACKGROUND_ANI_FIRST && helmet->background_mode <= BACKGROUND_ANI_LAST) {
        helmet->background_mode += number;
        if (helmet->background_mode < BACKGROUND_ANI_FIRST)
            helmet->background_mode = BACKGROUND_ANI_LAST;
        if (helmet->background_mode > BACKGROUND_ANI_LAST)
            helmet->background_mode = BACKGROUND_ANI_FIRST;
    }

    if (helmet->background_mode >= BACKGROUND_FFT_FIRST && helmet->background_mode <= BACKGROUND_FFT_LAST) {
        helmet->background_mode = random8(BACKGROUND_NOISE_FIRST, BACKGROUND_NOISE_LAST + 1);  // exit to random sound mode
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
            helmet->PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, w, w, b, b, b, b, b, bl);
            helmet->PaletteAniTarget = CRGBPalette16(w, w, w, b, b, b, b, b, w, w, w, b, b, b, b, b);
            helmet->color1Target = b_hsv;
            helmet->color2Target = w_hsv;

            break;
        case 1:
            helmet->PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, w, a, g, g, g, r, bl, bl);
            helmet->PaletteAniTarget = CRGBPalette16(w, a, g, g, r, w, a, g, g, g, r, w, a, g, g, r);
            helmet->color1Target = g_hsv;
            helmet->color2Target = a_hsv;

            break;
        case 2:
            helmet->PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, a, p, pi, pi, pi, r, bl, bl);
            helmet->PaletteAniTarget = CRGBPalette16(a, p, pi, pi, r, a, p, pi, pi, pi, r, a, p, pi, pi, r);
            helmet->color1Target = pi_hsv;
            helmet->color2Target = a_hsv;

            break;
        case 3:
            helmet->PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, g, a, b, b, b, b, bl, bl);
            helmet->PaletteAniTarget = CRGBPalette16(g, a, b, b, b, g, a, b, b, b, g, a, b, b, b, b);
            helmet->color1Target = b_hsv;
            helmet->color2Target = g_hsv;

            break;
        case 4:
            helmet->PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, p, p, y, y, bl, bl, bl, bl);
            helmet->PaletteAniTarget = CRGBPalette16(p, p, y, y, p, p, y, y, p, p, y, y, p, p, y, y);
            helmet->color1Target = p_hsv;
            helmet->color2Target = y_hsv;

            break;
        case 5:
            helmet->PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, a, a, y, y, y, y, bl, bl);
            helmet->PaletteAniTarget = CRGBPalette16(a, a, y, y, y, a, a, y, y, y, y, a, a, y, y, y);
            helmet->color1Target = a_hsv;
            helmet->color2Target = y_hsv;

            break;
        case 6:
            helmet->PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, b, r, bl, bl, bl, bl, bl, bl);
            helmet->PaletteAniTarget = CRGBPalette16(b, r, b, r, b, r, b, r, b, r, b, r, b, r, b, r);
            helmet->color1Target = r_hsv;
            helmet->color2Target = b_hsv;

            break;
        case 7:
            helmet->PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, o, o, p, bl, bl, bl, bl, bl);
            helmet->PaletteAniTarget = CRGBPalette16(o, o, p, o, o, p, o, o, p, o, o, p, o, o, p, o);
            helmet->color1Target = o_hsv;
            helmet->color2Target = p_hsv;

            break;
        case 8:
            helmet->PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, r, o, y, g, bl, bl, bl, bl);
            helmet->PaletteAniTarget = CRGBPalette16(r, o, y, g, r, o, y, g, r, o, y, g, r, o, y, g);
            helmet->color1Target = r_hsv;
            helmet->color2Target = y_hsv;

            break;
        case 9:
            helmet->PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, b, b, a, ra, ra, bl, bl, bl);
            helmet->PaletteAniTarget = CRGBPalette16(b, b, a, ra, ra, b, b, a, ra, ra, b, b, a, ra, ra, b);
            helmet->color1Target = b_hsv;
            helmet->color2Target = ra_hsv;

            break;
        case 10: {
            uint8_t temp1 = random8();
            uint8_t temp2 = random8();
            helmet->PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, r, r, temp1, temp2, bl, bl, bl, bl);
            helmet->PaletteAniTarget = CRGBPalette16(r, r, temp1, temp2, r, r, temp1, temp2, r, r, temp1, temp2, r, r, temp1, temp2);
            helmet->color1Target = r_hsv;
            helmet->color2Target = CHSV(temp1, 255, 255);
        } break;
        case 11:
            helmet->PaletteNoiseTarget = CRGBPalette16(bl, bl, bl, bl, bl, bl, bl, bl, b, p, pi, r, bl, bl, bl, bl);
            helmet->PaletteAniTarget = CRGBPalette16(b, p, pi, r, b, p, pi, r, b, p, pi, r, b, p, pi, r);
            helmet->color1Target = b_hsv;
            helmet->color2Target = r_hsv;

            break;
    }

    if (immediate) {
        helmet->color1 = helmet->color1Target;
        helmet->color2 = helmet->color2Target;
        nblendPaletteTowardPalette(helmet->PaletteNoiseCurrent, helmet->PaletteNoiseTarget, 255);
        nblendPaletteTowardPalette(helmet->PaletteAniCurrent, helmet->PaletteAniTarget, 255);
    }

    palette_change_time = millis();
}
void pattern_auto_update(struct popstar_struct *helmet) {
    // dont advance timers when off
    if (helmet->menu_state == 0) {
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
}