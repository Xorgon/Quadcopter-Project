/*
 * A sketch to log all Serial data to an SD card.
 */

#include <Arduino.h>
#include "Logger.h"

volatile unsigned long lastPWMTime;
volatile uint16_t pwmValue;

#define AUTOPILOT_ACTIVATED_PIN 3
#define ACTIVE_PWM 1900
#define ACTIVE_PWM_TOLERANCE 100

Logger logger;

void setup() {
    Serial.begin(19200);
    logger = Logger(4);

    attachInterrupt(0, onRising, RISING);
}

void loop() {
    if (Serial.available()) {
        String toWrite = "";
        while (Serial.available()) {
            toWrite.concat(char(Serial.read()));
        }
        logger.log(toWrite);
        Serial.print("/"); // To acknowledge receipt.
    }
    logger.checkFlush();
}



static void onRising() {
    attachInterrupt(0, onFalling, FALLING);
    lastPWMTime = micros();
}

static void onFalling() {
    attachInterrupt(0, onRising, RISING);
    pwmValue = micros() - lastPWMTime;
    if (pwmValue > (ACTIVE_PWM - ACTIVE_PWM_TOLERANCE)
        and pwmValue < (ACTIVE_PWM + ACTIVE_PWM_TOLERANCE)) {
        digitalWrite(AUTOPILOT_ACTIVATED_PIN, HIGH);
    } else {
        digitalWrite(AUTOPILOT_ACTIVATED_PIN, LOW);
    }
}
