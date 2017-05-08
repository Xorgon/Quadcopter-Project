#include <Arduino.h>
#include <Autopilot.h>

volatile unsigned long lastPWMTime;
volatile uint16_t pwmValue;
volatile bool autopilotActive;

SerialLogger logger;
Autopilot autopilot;

void setup() {
    Serial.begin(9600);
    logger.sync = false;
    autopilot = Autopilot(&logger);
    autopilot.sendPWM(900, 1000, 1100, 1200);
}

void loop() {
    autopilot.sendPWM(900, 1000, 1100, 1200);
    delay(1000);
    autopilot.sendPWM(1500, 1500, 1500, 1500);
    delay(1000);
}