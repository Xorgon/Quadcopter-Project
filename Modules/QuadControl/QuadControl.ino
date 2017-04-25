#include <Arduino.h>
#include "Autopilot.h"
#include "Grabber.h"
#include "Instruments.h"

Grabber grabber;
Autopilot autopilot;
Instruments instruments;
SerialLogger logger;

float tar[3];
float pos[3];

float yawTar;
float yaw;

// For Autopilot.
volatile unsigned long lastPWMTime;
volatile uint16_t pwmValue;
volatile bool autopilotActive;

void setup() {
    logger = SerialLogger();
    autopilot = Autopilot(&logger);
    grabber = Grabber(999, &logger); //TODO: Set correct servo pin.
    instruments = Instruments(&logger, 3, 4); //TODO: Set correct softSerial pins.

    //TODO: Set these correctly.
    tar[0] = 1.0;
    tar[1] = 1.0;
    tar[2] = 1.0;
    yawTar = 0;

    // Ensure nothing happens until actual values are acquired.
    pos[0] = tar[0];
    pos[1] = tar[1];
    pos[2] = tar[2];
    yaw = yawTar;
}

void loop() {
    yaw = instruments.setPos(pos);
    autopilot.run(tar, pos, yawTar, yaw);
    grabber.run(pos, tar);
}