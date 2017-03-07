#include <Arduino.h>
#include "Autopilot.h"
#include "Logger.h"

Autopilot autopilot;
Logger logger;

void setup() {
    logger = Logger(10);
    autopilot = Autopilot(logger);
}

void loop() {

}