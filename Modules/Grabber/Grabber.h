//
// Created by Elijah on 07/03/2017.
//

#ifndef SKETCH_DISPENSE_H
#define SKETCH_DISPENSE_H

#include <Arduino.h>
#include <Servo.h>
#include "math.h"
#include "SerialLogger.h"

class Grabber {
public:
    Grabber();

    Grabber(int servoPin, SerialLogger *logger);

    void run(float pos[], float target[]);

private:

    // Positional tolerance in meters.
#define posTolerance 0.1

    Servo servo;
    SerialLogger *logger;

    void moveTo(int loc);

    void release();
};

#endif //SKETCH_DISPENSE_H
