#include <Arduino.h>
#include <cobs.h>
#include <FastCRC.h>
#include "coprocessor.h"

// global stats
static uint8_t coprocessor_crc_errors = 0;
static uint8_t coprocessor_framing_errors = 0;
static FastCRC8 CRC8;

//serial com data
#define INCOMING_BUFFER_SIZE 16
static uint8_t incoming_raw_buffer[INCOMING_BUFFER_SIZE];
static uint8_t incoming_index = 0;
static uint8_t incoming_decoded_buffer[INCOMING_BUFFER_SIZE];

void coprocessor_init(void)
{
	Serial1.begin(115200); //ESP32 Coprocessor for Wiimote & ADXL345
}

void coprocessor_update(coprocessor_struct *coprocessor)
{
	while (Serial1.available())
	{
		//read in a byte
		incoming_raw_buffer[incoming_index] = Serial1.read();

		//check for end of packet
		if (incoming_raw_buffer[incoming_index] == 0x00)
		{
			//try to decode
			uint8_t decoded_length = COBSdecode(incoming_raw_buffer, incoming_index, incoming_decoded_buffer);

			//check length of decoded data (cleans up a series of 0x00 bytes)
			if (decoded_length > 0)
			{
				//check for framing errors
				if (decoded_length != 9)
				{
					coprocessor_framing_errors++;
				}
				else
				{
					//check for crc errors
					byte crc = CRC8.maxim(incoming_decoded_buffer, decoded_length - 1);
					if (crc != incoming_decoded_buffer[decoded_length - 1])
					{
						coprocessor_crc_errors++;
					}
					else
					{
						coprocessor->head_pitch = incoming_decoded_buffer[0];
						coprocessor->head_pitch -= 90;
						coprocessor->head_roll = incoming_decoded_buffer[1];
						coprocessor->head_roll -= 90;
						coprocessor->head_tap = incoming_decoded_buffer[2];

						if ((incoming_decoded_buffer[3] & 0b10000000) == 0b10000000)
							coprocessor->wiimote_connected = true;
						else
							coprocessor->wiimote_connected = false;

						if ((incoming_decoded_buffer[3] & 0b10000000) == 0b10000000)
							coprocessor->wiimote_connected = true;
						else
							coprocessor->wiimote_connected = false;

						if ((incoming_decoded_buffer[3] & 0b00000001) == 0b00000001)
							coprocessor->wiimote_z = true;
						else
							coprocessor->wiimote_z = false;

						if ((incoming_decoded_buffer[3] & 0b00000010) == 0b00000010)
							coprocessor->wiimote_c = true;
						else
							coprocessor->wiimote_c = false;

						if ((incoming_decoded_buffer[4] & 0b00010000) == 0b00010000)
							coprocessor->wiimote_plus = true;
						else
							coprocessor->wiimote_plus = false;

						if ((incoming_decoded_buffer[4] & 0b00001000) == 0b00001000)
							coprocessor->wiimote_up = true;
						else
							coprocessor->wiimote_up = false;

						if ((incoming_decoded_buffer[4] & 0b00000100) == 0b00000100)
							coprocessor->wiimote_down = true;
						else
							coprocessor->wiimote_down = false;

						if ((incoming_decoded_buffer[4] & 0b00000010) == 0b00000010)
							coprocessor->wiimote_right = true;
						else
							coprocessor->wiimote_right = false;

						if ((incoming_decoded_buffer[4] & 0b00000001) == 0b00000001)
							coprocessor->wiimote_left = true;
						else
							coprocessor->wiimote_left = false;

						if ((incoming_decoded_buffer[5] & 0b10000000) == 0b10000000)
							coprocessor->wiimote_home = true;
						else
							coprocessor->wiimote_home = false;

						if ((incoming_decoded_buffer[5] & 0b00010000) == 0b00010000)
							coprocessor->wiimote_minus = true;
						else
							coprocessor->wiimote_minus = false;

						if ((incoming_decoded_buffer[5] & 0b00001000) == 0b00001000)
							coprocessor->wiimote_a = true;
						else
							coprocessor->wiimote_a = false;
						if ((incoming_decoded_buffer[5] & 0b00000100) == 0b00000100)
							coprocessor->wiimote_b = true;
						else
							coprocessor->wiimote_b = false;
						if ((incoming_decoded_buffer[5] & 0b00000010) == 0b00000010)
							coprocessor->wiimote_1 = true;
						else
							coprocessor->wiimote_1 = false;
						if ((incoming_decoded_buffer[5] & 0b00000001) == 0b00000001)
							coprocessor->wiimote_2 = true;
						else
							coprocessor->wiimote_2 = false;

						coprocessor->wiimote_pitch = incoming_decoded_buffer[6];
						coprocessor->wiimote_pitch -= 90;
						coprocessor->wiimote_roll = incoming_decoded_buffer[7];
						coprocessor->wiimote_roll -= 90;

						uint8_t rumble = 0x00;
						if (coprocessor->wiimote_rumble)
							rumble = 0xF0;

						Serial1.write((0x0F & coprocessor->wiimote_leds) | rumble); //send reply
					}
				}
			}

			//reset index
			incoming_index = 0;
		}
		else
		{
			//read data in until we hit overflow then start over
			incoming_index++;
			if (incoming_index == INCOMING_BUFFER_SIZE)
				incoming_index = 0;
		}
	}
}