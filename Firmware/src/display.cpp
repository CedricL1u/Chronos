#include <ESP32Servo.h>
#include <config.h>

//config
Servo Servo1;
Servo Servo2;
Servo Servo3;
Servo Servo4;
double servoAngleMultiplier = 37.0/18.0;

void displaySetup() {
	Servo1.attach(Servo1_Pin);
	Servo2.attach(Servo2_Pin);
	Servo3.attach(Servo3_Pin);
	Servo4.attach(Servo4_Pin);
}

void displayModule1(int number) {
    Servo1.write((number * 36)/servoAngleMultiplier);
}

void displayModule2(int number) {
    Servo2.write((number * 36)/servoAngleMultiplier);
}

void displayModule3(int number) {
    Servo3.write((number * 36)/servoAngleMultiplier);
}

void displayModule4(int number) {
    Servo4.write((number * 36)/servoAngleMultiplier);
}

// Splits a number into 4 digits and drives each servo.
// e.g. 2530 -> module1=2, module2=5, module3=3, module4=0  ("25:30")
void displayNumber(int fourDigit) {
    if (fourDigit > 9999) fourDigit = 9999;
    if (fourDigit < 0)    fourDigit = 0;
    displayModule1((fourDigit / 1000) % 10);
    displayModule2((fourDigit / 100)  % 10);
    displayModule3((fourDigit / 10)   % 10);
    displayModule4( fourDigit         % 10);
}


