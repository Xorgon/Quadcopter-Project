//
// Created by Elijah on 07/03/2017.
//

#ifndef SKETCH_DISPENSE_H
#define SKETCH_DISPENSE_H

#include <Arduino.h>
#include <ServoTimer2.h>
#include "math.h"
#include "SerialLogger.h"

#define SERVO_CLOSED_MICROS 875
#define SERVO_OPEN_MICROS 2300

class Grabber {
public:
    Grabber();

    Grabber(uint8_t servoPin, SerialLogger *logger, uint16_t closeDelay);

    void run(float pos[], float target[]);

    void release();

private:

    // Positional tolerance in meters.
#define posTolerance 0.05

    ServoTimer2 servo;
    SerialLogger *logger;

    uint16_t closeDelay;
    uint32_t releaseTime;

    bool dropped;

    void checkClose();

};

#endif //SKETCH_DISPENSE_H
