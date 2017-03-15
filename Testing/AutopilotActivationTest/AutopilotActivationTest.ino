#include <Arduino.h>
#include "Autopilot.h"
#include "Logger.h"

Logger logger;
Autopilot autopilot;

volatile unsigned long lastPWMTime;
volatile uint16_t pwmValue;
volatile bool autopilotActive;

Servo out;

void setup() {
    Serial.begin(9600);
    logger = Logger(10);
    autopilot = Autopilot(&logger);
    out.attach(4);
    autopilotActive = false;
    out.writeMicroseconds(1900);
}

void loop() {
    if (millis() % 1000 <= 15) {
        logger.log("PWM", String(pwmValue));
        logger.log(AUTOPILOT_LOGGER_TAG, String(autopilotActive));
        if (autopilotActive) {
            out.writeMicroseconds(1000);
        } else {
            out.writeMicroseconds(1900);
        }
        autopilot.run();
    }
}