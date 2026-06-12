#include <Arduino.h>
#include "modes.h"

// ---------------------------------------------------------------------------
// Timer mode (plain countdown)
//   Encoder turn  -> set the duration (in minutes). Works before starting and
//                    also adjusts time mid-countdown.
//   Encoder click -> pause / resume (also acknowledges the alarm)
//   Switch 2      -> unused
// ---------------------------------------------------------------------------

static long    secondsLeft = 5 * 60;  // default 5:00
static bool    running     = false;
static bool    ringing     = false;
static unsigned long lastTick = 0;

static void render() {
    int mins = secondsLeft / 60;
    int secs = secondsLeft % 60;
    displayNumber(mins * 100 + secs);
}

void timerSetup() {
    secondsLeft = 5 * 60;
    running = false;
    ringing = false;
}

void timerEnter() {
    render();
}

void timerLoop() {
    if (!running) return;

    unsigned long now = millis();
    if (now - lastTick >= 1000) {
        lastTick = now;
        secondsLeft--;

        if (secondsLeft <= 0) {
            secondsLeft = 0;
            running = false;
            ringing = true;
            startAlarm();
        }
        render();
    }
}

void timerEncoderTurn(int direction) {
    // Adjust in 1-minute steps; allowed both before and during the countdown.
    long mins = secondsLeft / 60;
    long secs = secondsLeft % 60;
    mins = constrain(mins + direction, 0, 99);
    secondsLeft = mins * 60 + secs;
    render();
}

void timerEncoderClick() {
    if (ringing) {
        ringing = false;
        stopAlarm();
        running = false;
        render();
        return;
    }
    running = !running;
    if (running) lastTick = millis();
}

void timerSwitch2() {
    // Unused in timer mode (see CLAUDE.md).
}
