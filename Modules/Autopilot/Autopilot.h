//
// Created by Elijah on 16/02/2017.
//

#ifndef QUADCOPTER_TESTING_AUTOPILOT_H
#define QUADCOPTER_TESTING_AUTOPILOT_H

#include <Arduino.h>
#include <Logger.h>
#include <math.h>

class Autopilot {
public:
    Autopilot();

    Autopilot(Logger logger);

    float* calculate(float target[], float location[]);

private:
    float maxPitch;
    float maxRoll;
    float maxThrottle;
    float Kp[];
    float Kd[];


    float lastErrX;
    float lastErrY;
    float lastErrZ;

    Logger logger;
    String loggerTag = "Autopilot";

    int activePWM = 1111; //TODO: Find actual value.
    int lastPWMTime = 0;
    int pwmValue = 0;
    int pwmTolerance = 5; //TODO: Calibrate.

    void sendToController(float pitch, float roll, float yaw, float throttle);

    void onRising();
    void onFalling();

};


#endif //QUADCOPTER_TESTING_AUTOPILOT_H
