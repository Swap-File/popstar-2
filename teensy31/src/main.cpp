#include <Arduino.h>

#define USE_OCTOWS2811
#include <OctoWS2811.h>

#include "FastLED.h"
#include "Metro.h"
#include "main.h"
#include "outline.h"
#include "pattern.h"
#include "render.h"
#include "state.h"

static Metro timer_100hz = Metro(10, 1);
static Metro timer_10hz = Metro(100, 1);
static Metro timer_1hz = Metro(1000, 1);
static popstar_struct helmet;

static CRGB Output_Array[(STRIP_LEN)*8];  // actual output for Fastled with OctoWS2811

void setup() {
    FastLED.addLeds<OCTOWS2811>(Output_Array, STRIP_LEN);
    // FastLED.setBrightness(128);
    Serial.begin(115200);  // debug
    fft_init();
    cpu2_init();
    pinMode(FAN_PIN, OUTPUT);
    analogWriteFrequency(FAN_PIN, 187500);  // from https://www.pjrc.com/teensy/td_pulse.html
    analogWriteResolution(8);               // 0 - 255
}

void loop() {
    static int fps_led = 0;
    static int fps_fft = 0;
    cpu2_update(&(helmet.cpu2));
    bool fresh_fft = fft_update(&(helmet.fft));
    if (fresh_fft)
        fps_fft++;
    pattern_update(&helmet, helmet.pattern_array, fresh_fft, helmet.chin_array);

    outline_update(&helmet);
    // do hand updates;
    state_update(&helmet);

    int pattern_fps = fps_fft;
    int chin_fps = fps_fft;

    if (helmet.pattern_mode > PATTERN_NOISE_LAST)
        pattern_fps = 0;

    if (helmet.chin_mode == CHIN_CLONE_MID || helmet.chin_mode == CHIN_CLONE_MID )
        chin_fps = 0;

    //decay the snapshot
    cpu2_set_leds((0b0000001 & (fps_led >> 3)) | (0b00000010 & (pattern_fps >> 2)) | (0b00000100 & (chin_fps >> 2)) |  ( (0b0000001 & (fps_led >> 4) &helmet.spotlight) << 3) );

    if (timer_100hz.check()) {
        fps_led++;

        render(Output_Array, &helmet);

        FastLED.show();
        delay(5);  //dirty hack to prevent flickering
    }

    pattern_auto_update(&helmet);

    // snapshot the last image if we changed modes
    render_snapshot_check(&helmet);

    helmet.pattern_mode_last = helmet.pattern_mode;
    helmet.state_last = helmet.state;
    helmet.line_state_last = helmet.line_state;
    memcpy(&helmet.cpu2_last, &helmet.cpu2, sizeof(cpu2_struct));
}
