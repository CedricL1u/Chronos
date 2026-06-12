#include "Arduino.h"
#include "Audio.h"
#include "LittleFS.h"
#include "config.h"

// The path to the alarm sound file on the ESP32's flash (LittleFS).
// Upload your file with: PlatformIO -> Project Tasks -> Build Filesystem Image -> Upload Filesystem Image
// Make sure the file lives in a "data/" folder at the project root, e.g. data/alarm.mp3
#define ALARM_FILE "/alarm.mp3"

Audio audio;

// Tracks whether we want the alarm to keep looping until the user stops it.
static bool alarmLooping = false;

void audioSetup() {
    // Mount the flash filesystem that holds the audio file.
    if (!LittleFS.begin()) {
        Serial.println("LittleFS mount failed");
        return;
    }

    // Tell the Audio library which I2S pins to use (BCLK, LRCLK, DOUT/DIN).
    audio.setPinout(I2S_BCLK_Pin, I2S_LRCLK_Pin, I2S_DOUT_Pin);
    audio.setVolume(12); // 0...21
}

// Start playing the alarm sound. It will loop until stopAlarm() is called.
void startAlarm() {
    alarmLooping = true;
    audio.connecttoFS(LittleFS, ALARM_FILE);
}

// Stop the alarm immediately.
void stopAlarm() {
    alarmLooping = false;
    audio.stopSong();
}

// Must be called continuously from the main loop() so the library can
// stream audio data. Without this, nothing plays.
void audioLoop() {
    audio.loop();

    // When a play-through finishes, restart it so the alarm keeps ringing
    // until the user presses the stop button.
    if (alarmLooping && !audio.isRunning()) {
        audio.connecttoFS(LittleFS, ALARM_FILE);
    }
}
