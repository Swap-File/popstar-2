
#include "cpu2.h"

#include <Arduino.h>

#include "FastCRC.h"
#include "cobs.h"

// global stats
static uint8_t cpu2_crc_errors = 0;
static uint8_t cpu2_framing_errors = 0;
static FastCRC8 CRC8;

// serial com data
#define INCOMING_BUFFER_SIZE 16
static uint8_t incoming_raw_buffer[INCOMING_BUFFER_SIZE];
static uint8_t incoming_index = 0;
static uint8_t incoming_decoded_buffer[INCOMING_BUFFER_SIZE];

void cpu2_init(void) {
    Serial1.begin(115200);  // ESP32 cpu2 for Wiimote & ADXL345
}

void cpu2_update(struct cpu2_struct *cpu2) {
    while (Serial1.available()) {
        // read in a byte
        incoming_raw_buffer[incoming_index] = Serial1.read();

        // check for end of packet
        if (incoming_raw_buffer[incoming_index] == 0x00) {
            // try to decode
            uint8_t decoded_length = COBSdecode(incoming_raw_buffer, incoming_index, incoming_decoded_buffer);

            // check length of decoded data (cleans up a series of 0x00 bytes)
            if (decoded_length > 0) {
                // check for framing errors
                if (decoded_length != 9) {
                    cpu2_framing_errors++;
                } else {
                    // check for crc errors
                    byte crc = CRC8.maxim(incoming_decoded_buffer, decoded_length - 1);
                    if (crc != incoming_decoded_buffer[decoded_length - 1]) {
                        cpu2_crc_errors++;
                    } else {
                        cpu2->head_pitch = incoming_decoded_buffer[0];
                        cpu2->head_pitch -= 90;
                        cpu2->head_roll = incoming_decoded_buffer[1];
                        cpu2->head_roll -= 90;
                        cpu2->head_tap = incoming_decoded_buffer[2];

                        if ((incoming_decoded_buffer[3] & 0b10000000) == 0b10000000)
                            cpu2->wiimote_connected = true;
                        else
                            cpu2->wiimote_connected = false;

                        if ((incoming_decoded_buffer[3] & 0b10000000) == 0b10000000)
                            cpu2->wiimote_connected = true;
                        else
                            cpu2->wiimote_connected = false;

                        if ((incoming_decoded_buffer[3] & 0b00000001) == 0b00000001)
                            cpu2->wiimote_z = true;
                        else
                            cpu2->wiimote_z = false;

                        if ((incoming_decoded_buffer[3] & 0b00000010) == 0b00000010)
                            cpu2->wiimote_c = true;
                        else
                            cpu2->wiimote_c = false;

                        if ((incoming_decoded_buffer[4] & 0b00010000) == 0b00010000)
                            cpu2->wiimote_plus = true;
                        else
                            cpu2->wiimote_plus = false;

                        if ((incoming_decoded_buffer[4] & 0b00001000) == 0b00001000)
                            cpu2->wiimote_up = true;
                        else
                            cpu2->wiimote_up = false;

                        if ((incoming_decoded_buffer[4] & 0b00000100) == 0b00000100)
                            cpu2->wiimote_down = true;
                        else
                            cpu2->wiimote_down = false;

                        if ((incoming_decoded_buffer[4] & 0b00000010) == 0b00000010)
                            cpu2->wiimote_right = true;
                        else
                            cpu2->wiimote_right = false;

                        if ((incoming_decoded_buffer[4] & 0b00000001) == 0b00000001)
                            cpu2->wiimote_left = true;
                        else
                            cpu2->wiimote_left = false;

                        if ((incoming_decoded_buffer[5] & 0b10000000) == 0b10000000)
                            cpu2->wiimote_home = true;
                        else
                            cpu2->wiimote_home = false;

                        if ((incoming_decoded_buffer[5] & 0b00010000) == 0b00010000)
                            cpu2->wiimote_minus = true;
                        else
                            cpu2->wiimote_minus = false;

                        if ((incoming_decoded_buffer[5] & 0b00001000) == 0b00001000)
                            cpu2->wiimote_a = true;
                        else
                            cpu2->wiimote_a = false;
                        if ((incoming_decoded_buffer[5] & 0b00000100) == 0b00000100)
                            cpu2->wiimote_b = true;
                        else
                            cpu2->wiimote_b = false;
                        if ((incoming_decoded_buffer[5] & 0b00000010) == 0b00000010)
                            cpu2->wiimote_1 = true;
                        else
                            cpu2->wiimote_1 = false;
                        if ((incoming_decoded_buffer[5] & 0b00000001) == 0b00000001)
                            cpu2->wiimote_2 = true;
                        else
                            cpu2->wiimote_2 = false;

                        cpu2->wiimote_pitch = incoming_decoded_buffer[6];
                        cpu2->wiimote_pitch -= 90;
                        cpu2->wiimote_roll = incoming_decoded_buffer[7];
                        cpu2->wiimote_roll -= 90;

                        uint8_t rumble = 0x00;
                        if (cpu2->wiimote_rumble) rumble = 0xF0;

                        Serial1.write((0x0F & cpu2->wiimote_leds) | rumble);  // send reply
                    }
                }
            }

            // reset index
            incoming_index = 0;
        } else {
            // read data in until we hit overflow then start over
            incoming_index++;
            if (incoming_index == INCOMING_BUFFER_SIZE) incoming_index = 0;
        }
    }
}