// Created by Elijah Andrews on behalf of Team A

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

    void moveTo(int loc);

    void release();

    void run(float pos[], float target[]);

private:
    int pos;
    // Positional tolerance in meters.
    float posTolerance = 0.1;
    Servo servo;
    Logger logger;
};

#endif //SKETCH_DISPENSE_H
