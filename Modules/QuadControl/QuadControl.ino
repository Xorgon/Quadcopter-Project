#include <Arduino.h>
#include "Autopilot.h"
//#include "Grabber.h"
#include "Instruments.h"

//Grabber grabber;
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
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);

    logger = SerialLogger(11);
    Serial.begin(9600);
    autopilot = Autopilot(&logger);
//    grabber = Grabber(7, &logger);
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
}

void loop() {
    // General loop running LED.
    digitalWrite(13, HIGH);

    yaw = instruments.setPos(pos);

    // Uncomment/comment these out to enable/disable hover only.
    pos[0] = tar[0];
    pos[1] = tar[1];

    autopilot.run(tar, pos, yawTar, yaw);
//    grabber.run(pos, tar);

    digitalWrite(13, LOW);

    if (autopilotActive) {
        digitalWrite(12, HIGH);
    }

}