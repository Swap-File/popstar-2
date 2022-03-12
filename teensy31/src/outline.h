#include <Arduino.h>

#include "FastLED.h"
#include "main.h"
#include "metro.h"

static Metro outline_100hz = Metro(10, 1);

void outline_update(struct popstar_struct *helmet) {
    static int fade_amt = 0;
    static uint8_t gHue = 0;
    EVERY_N_MILLISECONDS(20) { gHue++; }  // slowly cycle the "base color" through the rainbow

    if (outline_100hz.check()) {
        if (helmet->state == STATE_STARTING || helmet->state == STATE_STOPPING) {
            static uint8_t offset = 0;
            offset++;

            for (int i = 0; i < OUTLINE_VERTICAL; i++) {
                helmet->outline_vert_overlay[i] = CHSV(255 * i / OUTLINE_VERTICAL + offset, 255, 255);
            }
        }

        if (helmet->outline_mode == OUTLINE_BEAT_H) {
            byte dothue = 64;
            fade_amt = 20;
            for (int i = 0; i < 7; i++) {
                helmet->outer_horiz[beatsin16(i + 7, 0, OUTLINE_HORIZONTAL - 1)] |= CHSV(dothue, 200, 255);
                dothue += 32;
            }
        } else if (helmet->outline_mode == OUTLINE_BEAT_V) {
            byte dothue = 64;
            fade_amt = 20;
            for (int i = 0; i < 7; i++) {
                helmet->outline_vert[beatsin16(i + 7, 0, OUTLINE_VERTICAL - 1)] |= CHSV(dothue, 200, 255);
                dothue += 32;
            }
        } else if (helmet->outline_mode == OUTLINE_SINELON_V) {
            fade_amt = 20;
            int pos = beatsin16(13, 0, OUTLINE_VERTICAL);
            helmet->outline_vert[pos] += CHSV(gHue, 255, 192);
        } else if (helmet->outline_mode == OUTLINE_SINELON_H) {
            fade_amt = 20;
            int pos = beatsin16(13, 0, OUTLINE_HORIZONTAL);
            helmet->outer_horiz[pos] += CHSV(gHue, 255, 192);
        } else if (helmet->outline_mode == OUTLINE_CONFETTI_H) {
            fade_amt = 10;
            int pos = random16(OUTLINE_HORIZONTAL);
            helmet->outer_horiz[pos] += CHSV(gHue + random8(64), 200, 255);
        } else if (helmet->outline_mode == OUTLINE_CONFETTI_V) {
            fade_amt = 10;
            int pos = random16(OUTLINE_VERTICAL);
            helmet->outline_vert[pos] += CHSV(gHue + random8(64), 200, 255);
        }

        fadeToBlackBy(helmet->outline_vert, OUTLINE_VERTICAL, fade_amt);
        fadeToBlackBy(helmet->outer_horiz, OUTLINE_HORIZONTAL, fade_amt);

        // still faster than writing the LEDs individually and fast enough
        for (int i = 0; i < OUTLINE_VERTICAL; i++) {
            helmet->outline_vert_overlay[i].nscale8(252);
        }
    }
}