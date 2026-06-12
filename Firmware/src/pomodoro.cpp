#include <Arduino.h>
#include "modes.h"

// ---------------------------------------------------------------------------
// Pomodoro mode
//
// 4 cycles of: 25 min work -> 5 min short break.
// After the 4th cycle: a 30 min long break, then it loops back to cycle 1.
//
// The timer AUTO-PAUSES whenever a phase hits 0 and rings the alarm. The user
// presses the encoder to stop the alarm and start the next phase.
//   Encoder turn  -> adjust the *current* phase's duration (in minutes)
//   Encoder click -> pause / resume (also acknowledges the alarm)
//   Switch 2      -> skip to the next phase
// ---------------------------------------------------------------------------

enum Phase { WORK, SHORT_BREAK, LONG_BREAK };

static Phase   phase        = WORK;
static int     cycle        = 1;        // 1..4
static long    secondsLeft  = 25 * 60;  // time remaining in current phase
static bool    running      = false;    // counting down?
static bool    ringing      = false;    // alarm sounding, waiting for ack?
static unsigned long lastTick = 0;      // millis() of last 1-second decrement

// Default minutes for each phase, used when (re)starting a phase.
static int workMins  = 25;
static int shortMins = 5;
static int longMins  = 30;

// Returns how many seconds the current phase should start with.
static long phaseStartSeconds() {
    switch (phase) {
        case WORK:        return (long)workMins  * 60;
        case SHORT_BREAK: return (long)shortMins * 60;
        case LONG_BREAK:  return (long)longMins  * 60;
    }
    return 0;
}

// Loads the current phase's duration and pauses, ready to be resumed.
static void loadPhase() {
    secondsLeft = phaseStartSeconds();
    running = false;
}

// Advances WORK -> SHORT_BREAK -> (next cycle) WORK, and after cycle 4 a
// LONG_BREAK, then back to cycle 1.
static void advancePhase() {
    if (phase == WORK) {
        phase = (cycle >= 4) ? LONG_BREAK : SHORT_BREAK;
    } else { // was a break
        if (phase == LONG_BREAK) cycle = 1;
        else                     cycle++;
        phase = WORK;
    }
    loadPhase();
}

// Pushes the current time-remaining onto the display as MM:SS digits.
static void render() {
    int mins = secondsLeft / 60;
    int secs = secondsLeft % 60;
    displayNumber(mins * 100 + secs);
}

void pomodoroSetup() {
    phase = WORK;
    cycle = 1;
    loadPhase();
}

// Called every time we switch INTO pomodoro mode.
void pomodoroEnter() {
    render();
}

void pomodoroLoop() {
    if (!running) return;

    unsigned long now = millis();
    if (now - lastTick >= 1000) {
        lastTick = now;
        secondsLeft--;

        if (secondsLeft <= 0) {
            secondsLeft = 0;
            running = false;
            ringing = true;
            startAlarm();   // rings until the user presses the encoder
        }
        render();
    }
}

void pomodoroEncoderTurn(int direction) {
    // Don't allow editing the duration while it's actively counting.
    if (running) return;

    // Adjust the default for whichever phase we're on, then reload it.
    switch (phase) {
        case WORK:        workMins  = constrain(workMins  + direction, 1, 99); break;
        case SHORT_BREAK: shortMins = constrain(shortMins + direction, 1, 99); break;
        case LONG_BREAK:  longMins  = constrain(longMins  + direction, 1, 99); break;
    }
    secondsLeft = phaseStartSeconds();
    render();
}

void pomodoroEncoderClick() {
    if (ringing) {
        // Acknowledge the alarm and move on to the next phase.
        ringing = false;
        stopAlarm();
        advancePhase();
        running = true;          // auto-start the next phase
        lastTick = millis();
        render();
        return;
    }

    // Normal pause / resume.
    running = !running;
    if (running) lastTick = millis();
}

void pomodoroSwitch2() {
    // Skip to the next phase regardless of state.
    if (ringing) { ringing = false; stopAlarm(); }
    advancePhase();
    render();
}
