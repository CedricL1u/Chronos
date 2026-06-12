#include <config.h>
#include <IoAbstraction.h>
#include <IoAbstractionWire.h>
#include <Wire.h>
#include "modes.h"

// ---------------------------------------------------------------------------
// Input handling.
//
// IoAbstraction's callbacks have FIXED signatures (they can't take a "mode"
// argument), so each callback below looks at the global `currentMode` and
// forwards the event to the matching mode's handler.
//
// Wiring (per config.h / schematic):
//   Switch1 = P0  -> always "change mode"
//   Switch2 = P1  -> per-mode action (skip cycle / stop alarm / AM-PM / unused)
//   Encoder switch = P4 -> per-mode click
//   Encoder A/B    = P2/P3
// All are on the PCF8574 expander, active-low (pressed = GND), so we use
// pull-up switching.
// ---------------------------------------------------------------------------

PCF8574IoAbstraction ioExpander(PCF8574_ADDRESS, I2C_INT_Pin, &Wire);

// Switch 1: cycle through the three modes.
static void onSwitch1(pinid_t /*pin*/, bool heldDown) {
    if (heldDown) return; // act on release (a normal press), not on hold
    switch (currentMode) {
        case MODE_POMODORO: changeMode(MODE_CLOCK);    break;
        case MODE_CLOCK:    changeMode(MODE_TIMER);    break;
        case MODE_TIMER:    changeMode(MODE_POMODORO); break;
    }
}

// Switch 2: meaning depends on the active mode.
static void onSwitch2(pinid_t /*pin*/, bool heldDown) {
    if (heldDown) return;
    switch (currentMode) {
        case MODE_POMODORO: pomodoroSwitch2(); break;
        case MODE_CLOCK:    clockSwitch2();    break;
        case MODE_TIMER:    timerSwitch2();    break;
    }
}

// Encoder push-switch: per-mode click.
static void onEncoderClick(pinid_t /*pin*/, bool heldDown) {
    if (heldDown) return;
    switch (currentMode) {
        case MODE_POMODORO: pomodoroEncoderClick(); break;
        case MODE_CLOCK:    clockEncoderClick();    break;
        case MODE_TIMER:    timerEncoderClick();    break;
    }
}

// Encoder rotation. IoAbstraction gives us an absolute position; we convert it
// into a +1 / -1 direction by comparing against the previous reading.
static int lastEncoderValue = 0;
static void onEncoderChange(int newValue) {
    int direction = (newValue > lastEncoderValue) ? 1 : -1;
    lastEncoderValue = newValue;
    switch (currentMode) {
        case MODE_POMODORO: pomodoroEncoderTurn(direction); break;
        case MODE_CLOCK:    clockEncoderTurn(direction);    break;
        case MODE_TIMER:    timerEncoderTurn(direction);    break;
    }
}

void inputSetup() {
    Wire.begin(I2C_SDA_Pin, I2C_SCL_Pin);

    // Interrupt-driven switches on the expander, active-low (pull-up switching).
    switches.initialiseInterrupt(asIoRef(ioExpander), true);

    // true (last arg) = invert logic, matching active-low buttons.
    switches.addSwitch(Switch1_Pin,       onSwitch1,      NO_REPEAT);
    switches.addSwitch(Switch2_Pin,       onSwitch2,      NO_REPEAT);
    switches.addSwitch(Encoder_Switch,    onEncoderClick, NO_REPEAT);

    // Rotary encoder on A/B. Range is wide so it never clamps; we only use the
    // delta between readings. 0x7fff start keeps room to turn both directions.
    setupRotaryEncoderWithInterrupt(Encoder_A, Encoder_B, onEncoderChange);
    switches.changeEncoderPrecision(0xffff, 0x7fff);
    lastEncoderValue = 0x7fff;
}
