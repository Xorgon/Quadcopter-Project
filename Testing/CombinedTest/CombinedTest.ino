#include <Arduino.h>
#include <Logger.h>
#include <Autopilot.h>
#include <Grabber.h>

Logger logger;
Autopilot autopilot;
Grabber grabber;

volatile unsigned long lastPWMTime;
volatile uint16_t pwmValue;
volatile bool autopilotActive;

void setup() {
    logger = Logger(10);
    autopilot = Autopilot(&logger);
    grabber = Grabber(4, &logger);
}

void loop() {

}