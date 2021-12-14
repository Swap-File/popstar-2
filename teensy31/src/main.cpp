#include <Arduino.h>

#include <Audio.h>
#define USE_OCTOWS2811
#include <OctoWS2811.h>
#include "FastLED.h"
#include "coprocessor.h"

CRGB Output_Array[145 * 8]; //actual output for Fastled with OctoWS2811

coprocessor_struct coprocessor;

void setup() {

  FastLED.addLeds<OCTOWS2811>(Output_Array, 145);
	FastLED.setBrightness(128);
	Serial.begin(115200); //debug

  coprocessor_init();

}

void loop() {
    
    coprocessor_update(&coprocessor);

    coprocessor.wiimote_rumble = coprocessor.wiimote_home;


}

