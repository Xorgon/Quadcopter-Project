#include <Arduino.h>
#include "Autopilot.h"
#include "Grabber.h"

Grabber grabber;
Autopilot autopilot;
SerialLogger logger;

float tar[3];
float loc[3];

float yawTar;
float yawLoc;

void setup() {
    logger = new SerialLogger();
    autopilot = new Autopilot(&logger);
    grabber = new Grabber(999, &logger); //TODO: Set correct servo pin.

    //TODO: Set these correctly.
    tar[0] = 1.0;
    tar[1] = 1.0;
    tar[2] = 1.0;

    // Instantiate instruments.
}

void loop() {
    // Get loc from the instruments.
    autopilot.run(&tar, &loc, yawTar, yaw);
    grabber.run(&loc, &tar);
}