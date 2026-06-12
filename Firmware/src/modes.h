#pragma once

// ---------------------------------------------------------------------------
// Shared declarations for the whole project.
//
// IoAbstraction's switch/encoder callbacks have FIXED signatures and cannot
// carry a "which mode are we in" argument. So the current mode is kept in one
// global (defined in main.cpp) and every input callback dispatches on it.
// ---------------------------------------------------------------------------

enum Mode {
    MODE_POMODORO,
    MODE_CLOCK,
    MODE_TIMER
};

// The single source of truth for the active mode. Defined in main.cpp.
extern Mode currentMode;

// Switch the active mode and run that mode's "enter" hook. Defined in main.cpp.
void changeMode(Mode newMode);

// ---- display.cpp ----------------------------------------------------------
void displaySetup();
void displayModule1(int number);
void displayModule2(int number);
void displayModule3(int number);
void displayModule4(int number);
// Shows a value as 4 digits across all servos, e.g. 2530 -> "25:30".
void displayNumber(int fourDigit);

// ---- input.ccp ------------------------------------------------------------
void inputSetup();

// ---- audio.cpp ------------------------------------------------------------
void audioSetup();
void audioLoop();
void startAlarm();
void stopAlarm();

// ---- pomodoro.cpp ---------------------------------------------------------
void pomodoroSetup();
void pomodoroEnter();
void pomodoroLoop();
void pomodoroEncoderTurn(int direction); // +1 / -1
void pomodoroEncoderClick();
void pomodoroSwitch2();

// ---- clock.cpp ------------------------------------------------------------
void clockSetup();
void clockEnter();
void clockLoop();
void clockEncoderTurn(int direction);
void clockEncoderClick();
void clockSwitch2();

// ---- timer.cpp ------------------------------------------------------------
void timerSetup();
void timerEnter();
void timerLoop();
void timerEncoderTurn(int direction);
void timerEncoderClick();
void timerSwitch2();
