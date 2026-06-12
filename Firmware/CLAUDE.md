# Chronos — Mechanical 7-Segment Display Pomodoro Timer

## Hardware Overview

**MCU:** Seeed XIAO ESP32-C3 (DIP breakout, U2)
**Power:** 5V 8A AC/DC wall plug → reverse polarity protection (AOD403 MOSFET) → bulk decoupling (2200uF cap)
**Servos:** 4× DS3235 (each draws 2A at stall — 8A supply sized for this)
**Audio:** 2× MAX98357A I2S mono amplifiers → 2× AS03104MR-F-WP-R speakers
**I/O Expander:** PCF8574AT (I2C, 8-bit) — switches and encoder pins are all routed through this; configure P5, P6, P7 as outputs
**Encoder:** EC11E18244A5 rotary encoder with switch
**Switches:** 2× linear Cherry MX-style mechanical switches (off-board, wire terminal, other pin tied to GND)

## Pin Assignments (see `src/config.h` for authoritative definitions)

**I2C:** SDA=D5, SCL=D6

**Servos:** Servo1=D1, Servo2=D2, Servo3=D9, Servo4=D8

**I2S Audio:** BCLK=D7, DIN=D3, LRCLK=D11

**PCF8574AT** (I2C address `0x38`): P0=Switch1, P1=Switch2, P2=Encoder_A, P3=Encoder_B, P4=Encoder_Switch

## Display Mapping

- Servo 1 → leftmost digit
- Servo 2 → second digit from left
- Servo 3 → third digit from left
- Servo 4 → rightmost digit

Each servo rotates a shaft 0–270°, geared 37:18 to the display shaft. Every 36° of shaft rotation = one digit (0–9 maps to 0°–324°).

## Build

```
pio run                        # build
pio run --target upload        # flash
pio device monitor             # serial monitor
```

---

This is code for a Mechanical 7 Segment Dipslay that functions as a pomodoro timer, a regular clock, and a regular timer. It works through 4 display modules. Each one has 7 segments. Each segment gets rotated back and forward by a rack and pinion. Attached to each segment is a gear which changes the angle of it from the rack and pinion. When the rack extends, the segment rotates 90 degrees and causes it to not be in display. When the rack retracts the segment rotates 90 degrees back and becomes in display. Now each segment has it's own rack so there are 7 racks. Each rack is powered by 2 cams. There's a cam that extends, and a cam that retracts. All 14 of the cams are all on a single shaft and the geometries work together such that every 36 degrees you rotate a different number is displayed. So at 0 degrees 0 is displayed at 36 degrees 1 is displayed and it goes all the way up to 324 degrees where the number 9 is displayed. Each display module is powered by a servo that rotates the main shaft that controls the number the display module and the segments show. This servo has a maximum angle of 270 degrees so its geared to the shaft at a 37:18 ratio from the servo to the shaft. Now servo 1 powers the display module on the very left and servo 4 powers the display module on the very right. Servo 2 powers the display module on the right of servo 1. servo 3 powers the display module on the right of servo 2. Now that you get how it works mechanically. I'll go into the other parts. This clock also includes a rotary encoder switch, specifically the EC11E18244A5, and two linear cherry mx style switches. These are inputs for the user on the design. The clock has 3 functions. Default is pomodoro mode. In pomdoro mode the clock has a pomdoro timer. It gives you 4 cyles of a 25 work timer followed by a 5 minute break period. After 4 cycles you get a 30 minute break. In terms of controls during this pomdoro mode. I want the rotary encoder switch click function to be a pause and play button. After every timer ends like the 25 minute timer or the 5 minute timer, the timer should automatically pause. The user should have to press the rotary encoder switch again to resume and go on to the next stage. I want the rotary encoder knob itself to be something that can change the timer of each session. I want one of the switches to allow you to switch between modes. I want the other switch to be able to let you skip between cycles in pomdoro mode. Now the second mode is clock mode. This is pretty self explanatory, I want you to use the esp32 connected to wifi to get the time and display it. There should also be an alarm feature. The rotary encoder knob should rotate to switch time and when u click it should set the alarm time. Switch 2 should toggle AM/PM when setting the alarm time. When the alarm goes off and is ringing, pressing switch 2 will end/stop the alarm. Now in timer mode the rotary encoder rotating should set the time duration. The clicking of the rotary encoder switch should pause and resume. Switch 1 should switch to a different mode. Switch 2 is not used in timer mode. There are also speakers on the board, they are two MAX98357A audio drivers which each drive a AS03104MR-F-WP-R speaker. In pomdoro mode it should play a alarm sound when each timer ends. This alarm sound shouldn't end until the user presses the rotary encoder. This feature should be in both alarm and timer mode. To be clear alarm mode isn't a seperate  mode its just built into clock mode. This is pretty much it. Also the switches and rotary encoder switch (encoder pins and switch pins) are all on a pcf9574at io expander. 