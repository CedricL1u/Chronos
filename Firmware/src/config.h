#pragma once

// I2C
#define I2C_SDA_Pin D4
#define I2C_SCL_Pin D5
#define I2C_INT_Pin D3
#define PCF8574_ADDRESS 0x38

//Servos
#define Servo1_Pin D0
#define Servo2_Pin D1
#define Servo3_Pin D9
#define Servo4_Pin D8

//Switches
#define Switch1_Pin 0
#define Switch2_Pin 1
#define Encoder_Switch 4

//Rotary Encoder
#define Encoder_A 2
#define Encoder_B 3

//Speakers
#define I2S_BCLK_Pin D6
#define I2S_DOUT_Pin D3
#define I2S_LRCLK_Pin D2

//WiFi + time (clock mode)
#define WIFI_SSID     "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
// NTP / timezone. GMT offset in seconds (e.g. EST = -5*3600), DST offset in seconds.
#define NTP_SERVER       "pool.ntp.org"
#define GMT_OFFSET_SEC   (-5 * 3600)
#define DST_OFFSET_SEC   (3600)
