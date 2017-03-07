//
// Created by Elijah on 07/03/2017.
//

#ifndef SKETCH_DISPENSE_H
#define SKETCH_DISPENSE_H

#include <Arduino.h>
#include <Servo.h>
#include "math.h"
#include "Logger.h"

class Grabber {
public:
    Grabber();

    Grabber(int servoPin, Logger logger);

    void run(float pos[], float target[]);

private:
    int pos;

    // Positional tolerance in meters.
    float posTolerance = 0.1;

    Servo servo;
    Logger logger;

    void moveTo(int loc);

    void release();
};

#endif //SKETCH_DISPENSE_H
