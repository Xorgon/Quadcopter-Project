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

    void sendToController(float pitch, float roll, float yaw, float throttle);
};


#endif //QUADCOPTER_TESTING_AUTOPILOT_H
