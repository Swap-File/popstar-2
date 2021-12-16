#include <Arduino.h>

#define USE_OCTOWS2811
#include <OctoWS2811.h>

#include "FastLED.h"
#include "Metro.h"
#include "cpu2.h"
#include "fft.h"
#include "main.h"
#include "pattern.h"

static Metro timer_100hz = Metro(10, 1);

static CRGB Output_Array[145 * 8];  // actual output for Fastled with OctoWS2811
static cpu2_struct cpu2;

static popstar_struct helmet;

void setup() {
    FastLED.addLeds<OCTOWS2811>(Output_Array, 145);
    FastLED.setBrightness(128);
    Serial.begin(115200);  // debug
    fft_init();
    cpu2_init();
}

void loop() {
    cpu2_update(&cpu2);

    fft_update(&(helmet.fft));
    pattern_update(&helmet, helmet.Background_Array);

    // do hand updates;

    cpu2.wiimote_rumble = cpu2.wiimote_home;

    if (cpu2.wiimote_home) helmet.menu_state = true;

    // do outline stuff
    // Do Background & snapshot to output calculations

    // 100hz framerate
    if (timer_100hz.check()) {
        for (uint8_t y = 0; y < 16; y++) {
            for (uint8_t x = 0; x < 24; x++) {
                helmet.Snapshot_Array[x][y].nscale8(240);
            }
        }
        FastLED.show();
    }

    pattern_auto_update(&helmet);

    // snapshot the last image if we changed modes
    if (helmet.background_mode != helmet.background_mode_last)
        memcpy(helmet.Snapshot_Array, helmet.Background_Array, 24 * 16 * sizeof(CRGB));

    helmet.background_mode_last = helmet.background_mode;
    helmet.menu_state_last = helmet.menu_state;
}
