

#include <Arduino.h>

#include "FastLED.h"
#include "main.h"

void state_update(struct popstar_struct *helmet) {
    // helmet->cpu2.wiimote_rumble = helmet->cpu2.wiimote_b;

    static uint32_t outline_timer = 0;
    if (millis() - outline_timer > 15000) {
        outline_timer = millis();
    //    helmet->outline_mode++;
        if (helmet->outline_mode > OUTLINE_LAST)
            helmet->outline_mode = 0;
    }
 helmet->outline_mode = 2;

    if (helmet->cpu2_last.wiimote_left == 0 && helmet->cpu2.wiimote_left) {
        if (helmet->pattern_mode <= PATTERN_FFT_LAST)
            helmet->pattern_mode = constrain(random(PATTERN_NOISE_FIRST, PATTERN_NOISE_LAST+1), PATTERN_NOISE_FIRST, PATTERN_NOISE_LAST);
        else
            helmet->pattern_mode = constrain(random(PATTERN_FFT_FIRST, PATTERN_FFT_LAST+1), PATTERN_FFT_FIRST, PATTERN_FFT_LAST);
    }

    if (helmet->cpu2_last.wiimote_right == 0 && helmet->cpu2.wiimote_right) {
        helmet->pattern_mode = constrain(random(PATTERN_ANI_FIRST, PATTERN_LAST+1), PATTERN_ANI_FIRST, PATTERN_LAST);
    }

    if (helmet->cpu2_last.wiimote_down == 0 && helmet->cpu2.wiimote_down) {
        helmet->outline_mode++;
        if (helmet->outline_mode > OUTLINE_LAST)
            helmet->outline_mode = 0;
    }

    if (helmet->cpu2_last.wiimote_1 == 0 && helmet->cpu2.wiimote_1) {
        helmet->chin_mode++;                //cycle outline
        if (helmet->chin_mode > CHIN_LAST)  //  helmet->chin_mode = constrain(random(CHIN_FIRST, CHIN_LAST), CHIN_FIRST, CHIN_LAST);
            helmet->chin_mode = 0;
    }

    if (helmet->cpu2_last.wiimote_2 == 0 && helmet->cpu2.wiimote_2) {
        helmet->chin_mode++;  //cycle outline
        if (helmet->chin_mode > CHIN_LAST)
            helmet->chin_mode = 0;
    }

    if (helmet->cpu2_last.wiimote_up == 0 && helmet->cpu2.wiimote_up) {
        helmet->spotlight = !helmet->spotlight;
    }

    // do outline stuff
    // Do Background & snapshot to output calculations

    static uint8_t gesture_state = 0;

    static uint8_t val = 0;
    if (helmet->cpu2_last.wiimote_plus == 0 && helmet->cpu2.wiimote_plus && val < 255) {
        val += 32;
        analogWrite(22, val);
    }
    if (helmet->cpu2_last.wiimote_minus == 0 && helmet->cpu2.wiimote_minus && val > 0) {
        val -= 32;
        analogWrite(22, val);
    }

    if (helmet->state == STATE_STOPPED) {
        if (helmet->cpu2.wiimote_b && !helmet->cpu2_last.wiimote_b && helmet->cpu2.wiimote_home) {
            if (helmet->cpu2.wiimote_a) {
                cpu2_set_rumble(100, 100, 2);
                helmet->state = STATE_RUNNING;
            } else {
                gesture_state = 1;
                helmet->state = STATE_STARTING;
            }
        }
    }

    else if (helmet->state == STATE_STARTING) {
        if (gesture_state == 1 && helmet->cpu2.wiimote_pitch < -20) {
            cpu2_set_rumble(100, 0, 1);
            gesture_state = 2;
        }
        if (gesture_state == 2) {
            helmet->outline_vert_mask = constrain(map(helmet->cpu2.wiimote_pitch, -20, 20, 0, OUTLINE_VERTICAL), 0, OUTLINE_VERTICAL);
        }
        if (helmet->outline_vert_mask == OUTLINE_VERTICAL || !helmet->cpu2.wiimote_b) {
            cpu2_set_rumble(100, 0, 1);
            gesture_state = 0;
            helmet->state = STATE_RUNNING;
        }
    }

    else if (helmet->state == STATE_RUNNING) {
        if (helmet->cpu2.wiimote_b && !helmet->cpu2_last.wiimote_b && helmet->cpu2.wiimote_home) {
            if (helmet->cpu2.wiimote_a) {
                cpu2_set_rumble(100, 100, 2);
                helmet->state = STATE_STOPPED;
            } else {
                gesture_state = 1;
                helmet->state = STATE_STOPPING;
            }
        }
    }

    else if (helmet->state == STATE_STOPPING) {
        if (gesture_state == 1 && helmet->cpu2.wiimote_pitch > 20) {
            cpu2_set_rumble(100, 0, 1);
            gesture_state = 2;
        }
        if (gesture_state == 2) {
            helmet->outline_vert_mask = constrain(map(helmet->cpu2.wiimote_pitch, -20, 20, 0, OUTLINE_VERTICAL), 0, OUTLINE_VERTICAL);
        }

        if (helmet->outline_vert_mask == 0 || !helmet->cpu2.wiimote_b) {
            helmet->state = STATE_STOPPED;
            gesture_state = 0;
            cpu2_set_rumble(100, 0, 1);
        }
    }

    if (helmet->state == STATE_RUNNING)
        helmet->outline_vert_mask = OUTLINE_VERTICAL;
    if (helmet->state == STATE_STOPPED)
        helmet->outline_vert_mask = 0;
}