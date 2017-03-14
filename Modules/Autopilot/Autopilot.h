//
// Created by Elijah on 16/02/2017.
//

#ifndef QUADCOPTER_TESTING_AUTOPILOT_H
#define QUADCOPTER_TESTING_AUTOPILOT_H

#include <Arduino.h>
#include <Logger.h>
#include <math.h>
#include "Servo.h"

class Autopilot {
public:
    Autopilot();

    Autopilot(Logger *logger);

    float *calculate(float target[], float location[]);

private:

#define maxPitch 30
#define maxRoll 30
#define maxThrottle 100

#define KP_X 0.1
#define KP_Y 0.1
#define KP_Z 0.1

#define KD_X 0.3
#define KD_Y 0.3
#define KD_Z 0.3

    float lastErrX;
    float lastErrY;
    float lastErrZ;

    Logger *logger;

#define AUTOPILOT_LOGGER_TAG "Autopilot"

    uint16_t activePWM = 1900; //TODO: Test activePWM value.
    volatile unsigned long lastPWMTime = 0;
    volatile uint16_t pwmValue = 0;
    uint8_t pwmTolerance = 100;
    uint8_t activePin = 10;

    Servo pitchPWM;
    Servo yawPWM;
    Servo rollPWM;
    Servo throttlePWM;

    void sendToController(float pitch, float roll, float yaw, float throttle);

    void onRising();

    void onFalling();

};


#endif //QUADCOPTER_TESTING_AUTOPILOT_H
