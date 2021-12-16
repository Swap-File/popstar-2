#ifndef _FFT_h
#define _FFT_h

#include <Arduino.h>

struct fft_struct {
    uint16_t value_16_max[16];       // max vals for normalization over time
    uint32_t value_16_max_time[16];  // when maxval is hit
    uint8_t value_16[16];            // max vals for normalization over time
    uint8_t value_8[8];              // max vals for normalization over time
    uint8_t value_12[12];
    float band[16];  // holdover from noise effects, adjust noise to directly
                     // use fft bins for speedup
};

void fft_init(void);
bool fft_update(fft_struct *fft);

#endif
