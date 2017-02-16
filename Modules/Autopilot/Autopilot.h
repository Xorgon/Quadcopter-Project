//
// Created by Elijah on 16/02/2017.
//

#ifndef QUADCOPTER_TESTING_AUTOPILOT_H
#define QUADCOPTER_TESTING_AUTOPILOT_H

#include <Arduino.h>
#include <Logger.h>

class Autopilot {
public:
    Autopilot();

    Autopilot(Logger logger);

    void calculate(float target[], float location[]);

private:
    float maxPitch;
    float maxRoll;
    float maxThrottle;
    float Kp;
    float Kd;
    Logger logger;
    String loggerTag = "Autopilot";

    void sendToController(float pitch, float roll, float yaw, float throttle);
};


#endif //QUADCOPTER_TESTING_AUTOPILOT_H
