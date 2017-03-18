#include <Arduino.h>
#include <Autopilot.h>

volatile unsigned long lastPWMTime;
volatile uint16_t pwmValue;
volatile bool autopilotActive;

Logger logger;
Autopilot autopilot;

void setup() {
    logger = Logger(10);
    autopilot = Autopilot(&logger);

    autopilot.sendPWM(1750, 1650, 1450, 1200);
}

void loop() {

}