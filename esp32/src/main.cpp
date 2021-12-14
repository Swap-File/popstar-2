#include <Arduino.h>
#include "ESP32Wiimote.h"
#include "TinyWiimote.h"
#include <SparkFun_ADXL345.h> // SparkFun ADXL345 Library
#include <math.h>
#include <cobs.h>    //cobs encoder and decoder
#include <OneWire.h> //crc8

#define FILTER 0.90

ADXL345 adxl = ADXL345(); // USE FOR I2C COMMUNICATION
ESP32Wiimote wiimote;

uint32_t loop_start;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);

  Serial.println("ESP32Wiimote");

  Serial.println("SparkFun ADXL345 Accelerometer Hook Up Guide Example");
  Serial.println();

  adxl.powerOn();           // Power on the ADXL345
  adxl.setRangeSetting(16); // Give the range settings
                            // Accepted values are 2g, 4g, 8g or 16g
                            // Higher Values = Wider Measurement Range
                            // Lower Values = Greater Sensitivity

  adxl.setTapDetectionOnXYZ(0, 0, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)

  // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
  adxl.setTapThreshold(50);     // 62.5 mg per increment
  adxl.setTapDuration(15);      // 625 μs per increment
  adxl.setDoubleTapLatency(80); // 1.25 ms per increment
  adxl.setDoubleTapWindow(200); // 1.25 ms per increment

  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.doubleTapINT(1);

  wiimote.init();

  Serial.println("Started");
  loop_start = millis();
}

void loop()
{

  // ADXL START
  static uint8_t head_pitch;
  static uint8_t head_roll;
  static uint8_t head_tap = 0;

  int adxl_x_tmp, adxl_y_tmp, adxl_z_tmp;

  adxl.readAccel(&adxl_x_tmp, &adxl_y_tmp, &adxl_z_tmp); // Read the accelerometer values and store them in variables declared above x,y,z

  byte interrupts = adxl.getInterruptSource();

  if (adxl.triggered(interrupts, ADXL345_DOUBLE_TAP))
    head_tap++;

  double adxl_pitch = atan(adxl_x_tmp / sqrt(pow(adxl_y_tmp, 2) + pow(adxl_z_tmp, 2)));
  double adxl_roll = atan(adxl_y_tmp / sqrt(pow(adxl_x_tmp, 2) + pow(adxl_z_tmp, 2)));

  adxl_pitch = adxl_pitch * (180.0 / 3.14);
  adxl_roll = adxl_roll * (180.0 / 3.14);

  static double adxl_filtered_pitch = 0;
  static double adxl_filtered_roll = 0;

  adxl_filtered_pitch = adxl_filtered_pitch * (FILTER) + adxl_pitch * (1 - FILTER);
  adxl_filtered_roll = adxl_filtered_roll * (FILTER) + adxl_roll * (1 - FILTER);

  head_pitch = 180 - (int(adxl_filtered_pitch + 270)) % 180;
  head_roll = (int(adxl_filtered_roll + 270)) % 180;

  // WII MOTE START
  wiimote.task();

  static ButtonState button;
  static AccelState accel;

  bool got_data = false;
  if (wiimote.available() > 0)
  {
    button = wiimote.getButtonState();
    accel = wiimote.getAccelState();
    got_data = true;
  }

  double mote_pitch = atan((0x80 - accel.yAxis) / sqrt(pow((0x80 - accel.xAxis), 2) + pow((0x80 - accel.zAxis), 2)));
  double mote_roll = atan((0x80 - accel.xAxis) / sqrt(pow((0x80 - accel.yAxis), 2) + pow((0x80 - accel.zAxis), 2)));

  mote_pitch = mote_pitch * (180.0 / 3.14);
  mote_roll = mote_roll * (180.0 / 3.14);

  static double mote_filtered_pitch = 0;
  static double mote_filtered_roll = 0;

  mote_filtered_pitch = mote_filtered_pitch * (FILTER) + mote_pitch * (1 - FILTER);
  mote_filtered_roll = mote_filtered_roll * (FILTER) + mote_roll * (1 - FILTER);

  uint8_t hand_pitch = (int(mote_filtered_pitch + 270)) % 180;
  uint8_t hand_roll = 180 - (int(mote_filtered_roll + 270)) % 180;

  // DEBUG START
  Serial.print(head_pitch);
  Serial.print("\t ");
  Serial.print(head_roll);
  Serial.print("\t ");
  Serial.print(head_tap);
  Serial.print("\t ");

  Serial.print(check_connected());
  Serial.print("\t ");
  Serial.print((button & BUTTON_A) ? 'A' : '.');
  Serial.print((button & BUTTON_B) ? 'B' : '.');
  Serial.print((button & BUTTON_C) ? 'C' : '.');
  Serial.print((button & BUTTON_Z) ? 'Z' : '.');
  Serial.print((button & BUTTON_ONE) ? '1' : '.');
  Serial.print((button & BUTTON_TWO) ? '2' : '.');
  Serial.print((button & BUTTON_MINUS) ? '-' : '.');
  Serial.print((button & BUTTON_PLUS) ? '+' : '.');
  Serial.print((button & BUTTON_HOME) ? 'H' : '.');
  Serial.print((button & BUTTON_LEFT) ? '<' : '.');
  Serial.print((button & BUTTON_RIGHT) ? '>' : '.');
  Serial.print((button & BUTTON_UP) ? '^' : '.');
  Serial.print((button & BUTTON_DOWN) ? 'v' : '.');
  Serial.print("\t ");
  Serial.print(hand_pitch);
  Serial.print("\t ");
  Serial.print(hand_roll);
  Serial.print("\t ");

  // OUTPUT START
  uint8_t raw_buffer[14];

  raw_buffer[0] = head_pitch;
  raw_buffer[1] = head_roll;
  raw_buffer[2] = head_tap;

  raw_buffer[3] = 0xFF & (button >> 16);
  raw_buffer[4] = 0xFF & (button >> 8);
  raw_buffer[5] = 0xFF & (button >> 0);

  if (check_connected())
  {
    raw_buffer[3] |= 0b10000000;
  }
  else
  {
    raw_buffer[3] &= 0b01111111;
  }

  raw_buffer[6] = hand_pitch;
  raw_buffer[7] = hand_roll;

  raw_buffer[8] = OneWire::crc8(raw_buffer, 8);

  uint8_t encoded_buffer[10];
  uint8_t encoded_size = COBSencode(raw_buffer, 9, encoded_buffer);
  Serial1.write(encoded_buffer, encoded_size);
  Serial1.write(0x00);

  // connection delay
  static uint32_t connect_time = 0;
  static bool connection_ready = false;
  static bool last_connect = false;
  if (check_connected() == false)
  {

    connect_time = 0;
  }
  if (check_connected() && last_connect != check_connected())
  {
    connect_time = millis();
  }
  last_connect = check_connected();
  if (millis() - connect_time > 1000)
    connection_ready = true;
  else
    connection_ready = false;

  // incoming serial read

  if (Serial1.available())
  {
    uint8_t serial_in = Serial1.read();
    while (Serial1.available())
      serial_in = Serial1.read();
    if (got_data && connection_ready)
      public_led(serial_in & 0xF0, serial_in & 0x0F);
  }

  // DELAY 10MS
  loop_start += 10;
  uint32_t predicted_delay = loop_start - millis();
  Serial.println(predicted_delay);
  if (predicted_delay > 10)
    predicted_delay = 0;
  if (predicted_delay != 0)
    delay(predicted_delay);
  else
    loop_start = millis();
}

void SerialUpdate(void)
{
}
