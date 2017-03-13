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

    Autopilot(Logger logger);

    float *calculate(float target[], float location[]);

private:
    float maxPitch;
    float maxRoll;
    float maxThrottle;
    float Kp[3] = {0.1, 0.1, 0.1};
    float Kd[3] = {0.3, 0.3, 0.3};


    float lastErrX;
    float lastErrY;
    float lastErrZ;

    Logger logger;
    String loggerTag = "Autopilot";

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
