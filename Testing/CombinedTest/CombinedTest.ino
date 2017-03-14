#include <Arduino.h>
#include "Autopilot.h"
#include "Logger.h"

Logger logger;
Autopilot autopilot;

void setup() {
    Serial.begin(9600);
    Serial.println("1");
    logger = Logger(10);
    Serial.println("2");
    autopilot = Autopilot(&logger);
    Serial.println("3");
}

void loop() {
    if (millis() % 10000 == 0) {
        Serial.println("Loop.");
    }
    logger.log("Test", "Rolling...");
}