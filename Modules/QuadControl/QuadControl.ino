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

// Originally used for autopilot interrupt system, left for any future use.
volatile unsigned long lastPWMTime;
volatile uint16_t pwmValue;
volatile bool autopilotActive;

uint32_t lastLoopTime;

void setup() {
    pinMode(12, OUTPUT);

    // Initialize logger with status LED on pin 11.
    logger = SerialLogger(11);

    // Comment/uncomment to enable/disable tethered logging.
//    logger.sync = false;

    Serial.begin(9600);

    // Initialize autopilot with logger.
    autopilot = Autopilot(&logger);

    // Comment/uncomment to enable/disable autopilot active override.
//    autopilot.activeOverride = true;

    // Initialize grabber on pin 7 with logger and 3000ms delay between open and close.
    grabber = Grabber(7, &logger, 3000);

    // Initialize instruments with logger and MSP software serial on pins 8 and 9.
    instruments = Instruments(&logger, 8, 9);

    // Set target.
    tar[0] = 1.0;
    tar[1] = 1.0;
    tar[2] = 1.0;
    yawTar = -1;

    // Ensure nothing happens until actual values are acquired.
    pos[0] = tar[0];
    pos[1] = tar[1];
    pos[2] = tar[2];
    yaw = -1;
    lastLoopTime = 0;
}

void loop() {

    // Update position array.
    yaw = instruments.setPos(pos);

    // Comment/uncomment to enable/disable yaw processing.
    yawTar = yaw;

    if (yawTar == -1) {
        yawTar = yaw;
    }

    // Comment/uncomment these to disable/enable hover only.
    pos[0] = tar[0];
    pos[1] = tar[1];

    // Run autopilot.
    autopilot.run(tar, pos, yawTar, yaw);

    // Run grabber.
    grabber.run(pos, tar);

    // Log loops per second.
    logger.log(F("Sys"), "LPS: " + String(1000 / (millis() - lastLoopTime)));
    lastLoopTime = millis();
}