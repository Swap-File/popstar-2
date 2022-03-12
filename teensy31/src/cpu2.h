#ifndef _CPU2_h
#define _CPU2_h

#include <Arduino.h>

struct cpu2_struct {
    //inputs
    bool wiimote_connected = false;
    int wiimote_pitch = 0;
    int wiimote_roll = 0;
    bool wiimote_a = false;
    bool wiimote_b = false;
    bool wiimote_c = false;
    bool wiimote_z = false;
    bool wiimote_1 = false;
    bool wiimote_2 = false;
    bool wiimote_minus = false;
    bool wiimote_plus = false;
    bool wiimote_up = false;
    bool wiimote_down = false;
    bool wiimote_left = false;
    bool wiimote_right = false;
    bool wiimote_home = false;

    //inputs
    int head_pitch = 0;
    int head_roll = 0;
    int head_tap = 0;
};

void cpu2_update(struct cpu2_struct *cpu2);
void cpu2_init(void);
void cpu2_set_rumble(int duration_on, int duration_off, int cycles);
void cpu2_set_leds(uint8_t leds);

#endif
