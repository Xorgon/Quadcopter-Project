#include <Arduino.h>
#include "Servo.h"

Servo pwmOut;
uint32_t lastPWMTime;
uint16_t pwmValue;

void setup() {
    Serial.begin(9600);
    attachInterrupt(0, onRising, RISING);
    pwmOut.attach(13);
    pwmOut.writeMicroseconds(1900-4);
}

void loop() {
}

void onRising() {
    attachInterrupt(0, onFalling, FALLING);
    lastPWMTime = micros();
}

void onFalling() {
    attachInterrupt(0, onRising, RISING);
    pwmValue = micros() - lastPWMTime;
    Serial.println(pwmValue);
}