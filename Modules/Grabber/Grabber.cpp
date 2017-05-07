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
Grabber::Grabber(int servoPin, SerialLogger *logger) {
    servo = Servo();
    servo.attach(servoPin);
    servo.write(0 + SERVO_OFFSET);
    this->logger = logger;
    scheduler = Scheduler();
}

/**
 * Releases the grabber.
 */
void Grabber::release() {
    servo.write(140 + SERVO_OFFSET);
    scheduler.schedule(this->close); // TODO: Test scheduler.
}

void Grabber::close() {
    servo.write(0 + SERVO_OFFSET);
}

/**
 * Checks the position and, when at the target position, releases the grabber.
 * @param pos Position vector of the aircraft.
 * @param target Target position vector.
 */
void Grabber::run(float *pos, float *target) {
    scheduler.update();
    if (fabs(pos[0] - target[0]) < posTolerance,
            fabs(pos[1] - target[1]) < posTolerance,
            fabs(pos[2] - target[2]) < posTolerance) {
        release();
        logger->log(F("Grabber"), F("Released payload."));
    }
}