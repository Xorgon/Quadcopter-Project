// Created by Elijah Andrews on behalf of Team A

#include "Grabber.h"

Grabber::Grabber() {} // Allows the code to create a global Grabber variable without properly initializing.

Grabber::Grabber(int servoPin, Logger logger) {
    pos = 0;
    servo = Servo();
    servo.attach(servoPin);
    servo.write(0);
    this->logger = logger;
}

// Function to smoothly move the servo.
void Grabber::moveTo(int loc) {
    if (pos == loc) {
        return;
    }
    if (pos < loc) {
        for (pos; pos < loc; pos++) {
            servo.write(pos);
            delay(5);
        }
    } else {
        for (pos; pos > loc; pos--) {
            servo.write(pos);
            delay(5);
        }
    }
}

// Function to release.
void Grabber::release() {
    moveTo(90);
    delay(50);
    moveTo(0);
}

void Grabber::run(float *pos, float *target) {
    if (fabs(pos[0] - target[0]) < posTolerance,
            fabs(pos[1] - target[1]) < posTolerance,
            fabs(pos[2] - target[2]) < posTolerance) {
        release();
        logger.log("Grabber", "Released payload.");
    }
}