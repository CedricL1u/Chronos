#include <Arduino.h>
#include <IoAbstraction.h>   // for taskManager
#include "modes.h"

// The single source of truth for the active mode (declared extern in modes.h).
Mode currentMode = MODE_POMODORO;

// Switches mode and runs the new mode's "enter" hook so it can draw itself.
void changeMode(Mode newMode) {
    currentMode = newMode;
    switch (currentMode) {
        case MODE_POMODORO: pomodoroEnter(); break;
        case MODE_CLOCK:    clockEnter();    break;
        case MODE_TIMER:    timerEnter();    break;
    }
}

void setup() {
    Serial.begin(115200);

    displaySetup();
    inputSetup();
    audioSetup();

    pomodoroSetup();
    clockSetup();
    timerSetup();

    // Start in the default mode and draw it.
    changeMode(MODE_POMODORO);
}

void loop() {
    // IoAbstraction: services switches/encoder interrupts and callbacks.
    taskManager.runLoop();

    // Keep the audio library streaming (no-op when nothing is playing).
    audioLoop();

    // Run only the active mode's per-frame logic.
    switch (currentMode) {
        case MODE_POMODORO: pomodoroLoop(); break;
        case MODE_CLOCK:    clockLoop();    break;
        case MODE_TIMER:    timerLoop();    break;
    }
}
