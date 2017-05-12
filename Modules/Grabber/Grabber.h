//
// Created by Elijah on 07/03/2017.
//

// TODO: Test grabber 'twitchy-ness' on proper power.

#ifndef SKETCH_DISPENSE_H
#define SKETCH_DISPENSE_H

#include <Arduino.h>
#include <ServoTimer2.h>
#include "math.h"
#include "SerialLogger.h"

#define SERVO_OFFSET 4

class Grabber {
public:
    Grabber();

    Grabber(uint8_t servoPin, SerialLogger *logger, uint16_t closeDelay);

    void run(float pos[], float target[]);

    void release();

private:

    // Positional tolerance in meters.
#define posTolerance 0.1

    ServoTimer2 servo;
    SerialLogger *logger;

    uint16_t closeDelay;
    uint32_t releaseTime;

    bool dropped;

    void checkClose();

};

#endif //SKETCH_DISPENSE_H
