#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include "modes.h"
#include "config.h"

// ---------------------------------------------------------------------------
// Clock mode (+ built-in alarm)
//
// Normal:    shows the current time (HH:MM) fetched over WiFi via NTP.
//   Encoder click -> enter "set alarm" mode
//   Switch 2      -> (only while ringing) stop the alarm
//
// Setting alarm:
//   Encoder turn  -> change the alarm time (steps of 1 minute)
//   Encoder click -> save the alarm time, return to normal
//   Switch 2      -> toggle AM/PM of the alarm being set
// ---------------------------------------------------------------------------

static bool settingAlarm = false;
static bool alarmEnabled = false;
static bool ringing      = false;

// Alarm time stored as 24h minutes-since-midnight (0..1439).
static int  alarmMinutes = 7 * 60; // default 07:00
// AM/PM is just a view over alarmMinutes while editing.

static unsigned long lastRender = 0;
static int lastShownHHMM = -1;

// Converts minutes-since-midnight into a 4-digit HHMM (12-hour) value.
static int toHHMM_12(int minutesOfDay) {
    int h24 = minutesOfDay / 60;
    int m   = minutesOfDay % 60;
    int h12 = h24 % 12;
    if (h12 == 0) h12 = 12;
    return h12 * 100 + m;
}

static void renderCurrentTime() {
    struct tm t;
    if (!getLocalTime(&t, 100)) {
        // Time not synced yet; show 0:00 as a placeholder.
        displayNumber(0);
        return;
    }
    int hhmm = toHHMM_12(t.tm_hour * 60 + t.tm_min);
    if (hhmm != lastShownHHMM) {
        displayNumber(hhmm);
        lastShownHHMM = hhmm;
    }

    // Check the alarm: fire when current time matches and alarm is enabled.
    if (alarmEnabled && !ringing) {
        int nowMinutes = t.tm_hour * 60 + t.tm_min;
        if (nowMinutes == alarmMinutes && t.tm_sec == 0) {
            ringing = true;
            startAlarm();
        }
    }
}

void clockSetup() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // Wait briefly for a connection (non-fatal if it fails; we'll show 0:00).
    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 8000) {
        delay(100);
    }

    configTime(GMT_OFFSET_SEC, DST_OFFSET_SEC, NTP_SERVER);
}

void clockEnter() {
    settingAlarm = false;
    lastShownHHMM = -1; // force a redraw
    renderCurrentTime();
}

void clockLoop() {
    if (settingAlarm) {
        return; // display is showing the alarm time being edited
    }
    // Refresh ~once per second.
    if (millis() - lastRender >= 1000) {
        lastRender = millis();
        renderCurrentTime();
    }
}

void clockEncoderTurn(int direction) {
    if (!settingAlarm) return; // knob only edits time while setting alarm
    alarmMinutes = (alarmMinutes + direction + 1440) % 1440;
    displayNumber(toHHMM_12(alarmMinutes));
}

void clockEncoderClick() {
    if (ringing) return; // while ringing, only switch 2 stops it

    if (!settingAlarm) {
        // Enter set-alarm mode and show the alarm time.
        settingAlarm = true;
        displayNumber(toHHMM_12(alarmMinutes));
    } else {
        // Save and go back to showing the current time.
        settingAlarm = false;
        alarmEnabled = true;
        lastShownHHMM = -1;
        renderCurrentTime();
    }
}

void clockSwitch2() {
    if (ringing) {
        // Stop the alarm.
        ringing = false;
        stopAlarm();
        return;
    }
    if (settingAlarm) {
        // Toggle AM/PM by shifting the alarm 12 hours.
        alarmMinutes = (alarmMinutes + 12 * 60) % 1440;
        displayNumber(toHHMM_12(alarmMinutes));
    }
}
