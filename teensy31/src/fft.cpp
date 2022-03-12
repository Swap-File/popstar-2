#include "fft.h"

#include <Arduino.h>

#include "Audio.h"

static AudioInputAnalog adc1(A9);  // A9 is on ADC0
static AudioAnalyzeFFT256 fft256_1;
static AudioConnection patchCord1(adc1, fft256_1);

void fft_init(void) {
    AudioMemory(6);
    fft256_1.windowFunction(AudioWindowHanning256);
    fft256_1.averageTogether(4);
}


bool fft_update(fft_struct *fft) {

    if (fft256_1.available() == false) return false;

    for (uint8_t i = 0; i < 16; i++) {
        int16_t n = 1000 * fft256_1.read((i * 2), (i * 2) + 2);

        // de-emphasize lower frequencies
        switch (i) {
            case 0:
                n = max(n - 100, 0);
                break;
            case 1:
                n = max(n - 50, 0);
                break;
            case 2:
                n = max(n - 15, 0);
                break;
            case 3:
                n = max(n - 10, 0);
                break;
            default:
                n = max(n - 3, 0);
                break;
        }

        // falloff controll
        fft->value_16_max[i] = max(max(fft->value_16_max[i] * .97, n), 4);
        fft->value_16[i] =
            constrain(map(n, 0, fft->value_16_max[i], 0, 255), 0, 255);

        if (n == fft->value_16_max[i]) fft->value_16_max_time[i] = millis();

        // downsample 16 samples to 8
        if (i & 0x01) {
            fft->value_8[i >> 1] =
                (fft->value_16[i] + fft->value_16[i - 1]) >> 1;
        }
    }
    // downsample 16 samples to 12
    for (uint8_t i = 0; i < 8; i++) fft->value_12[i] = fft->value_16[i];
    fft->value_12[8] = (fft->value_16[8] + fft->value_16[9]) >> 1;
    fft->value_12[9] = (fft->value_16[10] + fft->value_16[11]) >> 1;
    fft->value_12[10] = (fft->value_16[12] + fft->value_16[13]) >> 1;
    fft->value_12[11] = (fft->value_16[14] + fft->value_16[15]) >> 1;

    for (uint8_t i = 0; i < 10; i++) {
        fft->band[i] = fft->value_16[i];
        fft->band[i] /= 400;
    }

    return true;
}