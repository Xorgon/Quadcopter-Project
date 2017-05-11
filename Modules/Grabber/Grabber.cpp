//
// Created by Elijah on 07/03/2017.
//

#include "Grabber.h"

/**
 * Allows for creation of a Grabber variable without initialization.
 */
Grabber::Grabber() {}

/**
 * Initializes Grabber object.
 * @param servoPin The pin to which the servo is connected.
 * @param logger The logger being used.
 */
Grabber::Grabber(uint8_t servoPin, SerialLogger *logger, uint16_t closeDelay) {
    servo = Servo();
    servo.attach(servoPin);
    servo.write(0 + SERVO_OFFSET);
    this->logger = logger;
    this->closeDelay = closeDelay;
    releaseTime = 0;
    dropped = false;
}

/**
 * Releases the grabber.
 */
void Grabber::release() {
    servo.write(140 + SERVO_OFFSET);
    releaseTime = millis();
    logger->log(F("Grabber"), F("Released payload."));
    dropped = true;
}

void Grabber::checkClose() {
    if (millis() - releaseTime > closeDelay && releaseTime != 0) {
        servo.write(0 + SERVO_OFFSET);
        releaseTime = 0;
        logger->log(F("Grabber"), F("Closed."));
    }
}

/**
 * Checks the position and, when at the target position, releases the grabber.
 * @param pos Position vector of the aircraft.
 * @param target Target position vector.
 */
void Grabber::run(float *pos, float *target) {
    checkClose();
    if (fabs(pos[0] - target[0]) < posTolerance &&
            fabs(pos[1] - target[1]) < posTolerance &&
            fabs(pos[2] - target[2]) < posTolerance && !dropped) {
        release();
    }
}