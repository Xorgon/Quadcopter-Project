//
// Created by Elijah on 16/02/2017.
//

#ifndef QUADCOPTER_TESTING_AUTOPILOT_H
#define QUADCOPTER_TESTING_AUTOPILOT_H

#include <Arduino.h>
#include <math.h>
#include <ServoTimer2.h>
#include "SerialLogger.h"

// These variables must be created in the main control file.
extern volatile bool autopilotActive;

class Autopilot {
public:
    Autopilot();

    Autopilot(SerialLogger *logger);

    void run(float tar[], float loc[], float yawTar, float yaw);

    uint16_t calculatePitch(float errX); // PD
    uint16_t calculateRoll(float errY); // PD
    uint16_t calculateYaw(float errYaw); // PI
    uint16_t calculateThrottle(float errZ); // PI

    void sendPWM(uint16_t pitch, uint16_t roll, uint16_t yaw, uint16_t throttle);

    bool activeOverride = false;

private:

    bool lastActive;

#define MAX_PITCH 50
#define MAX_ROLL 50
#define MAX_THROTTLE 50
#define MIN_THROTTLE 250
#define THROTTLE_CENTER 1400

#define KP_X 0.5
#define KP_Y 0.5
#define KP_Z 0.5
#define KP_YAW 0.5

#define KD_X 0.1
#define KD_Y 0.1
#define KD_Z 0.1

#define KI_Z 0.1
#define KI_YAW 0.1

#define ROLL_PD_PWM_FACTOR 50
#define PITCH_PD_PWM_FACTOR 50
#define YAW_PI_PWM_FACTOR 50
#define THROTTLE_PI_PWM_FACTOR 50

    float lastErrX;
    float lastErrY;
    float lastErrZ;

    uint32_t lastYawTime;
    uint32_t lastThrottleTime;

    float yawIntegral;
    float throttleIntegral;

    SerialLogger *logger;

#define AUTOPILOT_LOGGER_TAG "Autopilot"

#define ACTIVE_PWM 1900
#define ACTIVE_PWM_TOLERANCE 100
#define ACTIVE_PIN 10
#define ACTIVE_LED_PIN 12

#define PITCH_PWM_PIN 3
#define ROLL_PWM_PIN 4
#define YAW_PWM_PIN 5
#define THROTTLE_PWM_PIN 6

    ServoTimer2 pitchPWM;
    ServoTimer2 yawPWM;
    ServoTimer2 rollPWM;
    ServoTimer2 throttlePWM;

    void resetPID();

};


#endif //QUADCOPTER_TESTING_AUTOPILOT_H
