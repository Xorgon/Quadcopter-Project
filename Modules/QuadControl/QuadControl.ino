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

uint32_t lastLoopTime;

void setup() {
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    logger = SerialLogger(11);

    // Comment/uncomment to enable/disable tethered logging.
    logger.sync = false;

    Serial.begin(57600);
    autopilot = Autopilot(&logger);
    grabber = Grabber(7, &logger, 3000);
    instruments = Instruments(&logger, 8, 9);
    tar[0] = 1.0;
    tar[1] = 1.0;
    tar[2] = 0.8;
    yawTar = 0;

    // Ensure nothing happens until actual values are acquired.
    pos[0] = tar[0];
    pos[1] = tar[1];
    pos[2] = tar[2];
    yaw = yawTar;
    lastLoopTime = 0;
}

void loop() {
    // General loop running LED.
    //digitalWrite(13, HIGH);

    yaw = instruments.setPos(pos);

    // Comment/uncomment these out to enable/disable hover only.
    pos[0] = tar[0];
    pos[1] = tar[1];

    Serial.println(pwmValue);

    autopilot.run(tar, pos, yawTar, yaw);
    grabber.run(pos, tar);

    //digitalWrite(13, LOW);

    if (autopilotActive) {
        digitalWrite(12, HIGH);
    }
    logger.log("System", "TPS: " + String(1000/(millis() - lastLoopTime)));
    lastLoopTime = millis();
}