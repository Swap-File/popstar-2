#include <Arduino.h>

#define USE_OCTOWS2811
#include <OctoWS2811.h>

#include "FastLED.h"
#include "Metro.h"
#include "main.h"
#include "pattern.h"
#include "render.h"

static Metro timer_100hz = Metro(10, 1);
static Metro timer_10hz = Metro(100, 1);
static popstar_struct helmet;

static CRGB Output_Array[(STRIP_LEN)*8];  // actual output for Fastled with OctoWS2811

void setup() {
    FastLED.addLeds<OCTOWS2811>(Output_Array, STRIP_LEN);
    FastLED.setBrightness(128);
    Serial.begin(115200);  // debug
    fft_init();
    cpu2_init();
}

void loop() {
    cpu2_update(&(helmet.cpu2));
    bool fresh_fft = fft_update(&(helmet.fft));
    pattern_update(&helmet, helmet.Background_Array, fresh_fft, helmet.chin_array);

    // do hand updates;

    helmet.cpu2.wiimote_rumble = helmet.cpu2.wiimote_home;

    if (helmet.cpu2.wiimote_home) helmet.menu_state = true;

    // do outline stuff
    // Do Background & snapshot to output calculations

    //decay the snapshot
    if (timer_100hz.check()) {
        //render the scene
        render(Output_Array, &helmet);

        //let fastled run as fast as it can so it can do temporal dithering
        FastLED.show();

        // test animation

        fadeToBlackBy(helmet.outer_horiz, OUTLINE_HORIZONTAL, 20);

        byte dothue = 64;

        for (int i = 2; i < 7; i++) {
            helmet.outer_horiz[beatsin16(i + 7, 0, OUTLINE_HORIZONTAL -1)] |= CHSV(dothue, 200, 255);
            dothue += 32;
        }

        fadeToBlackBy(helmet.outer_vert, OUTLINE_VERTICAL, 20);
         byte dothue2 = 0;

        for (int i = 0; i <2; i++) {
            helmet.outer_vert[beatsin16(i + 7, 0, OUTLINE_VERTICAL -1)] |= CHSV(dothue2, 200, 255);
            dothue2 += 32;
        }

    }

    pattern_auto_update(&helmet);

    // snapshot the last image if we changed modes
    render_snapshot_check(&helmet);

    helmet.background_mode_last = helmet.background_mode;
    helmet.menu_state_last = helmet.menu_state;
    helmet.line_state_last = helmet.line_state;
}
