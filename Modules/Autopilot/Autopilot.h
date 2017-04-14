//
// Created by Elijah on 16/02/2017.
//

#ifndef QUADCOPTER_TESTING_AUTOPILOT_H
#define QUADCOPTER_TESTING_AUTOPILOT_H

#include <Arduino.h>
#include <LoggerLite.h>
#include <math.h>
#include "Servo.h"

// These variables must be created in the main control file.
extern volatile unsigned long lastPWMTime;
extern volatile uint16_t pwmValue;
extern volatile bool autopilotActive;

class Autopilot {
public:
    Autopilot();

    Autopilot(LoggerLite *logger);

    void run(float tar[], float loc[], float yawTar, float yaw);

    uint16_t calculatePitch(float errX); // PD
    uint16_t calculateRoll(float errY); // PD
    uint16_t calculateYaw(float errYaw); // PI
    uint16_t calculateThrottle(float errZ); // PI

    void sendPWM(uint16_t pitch, uint16_t roll, uint16_t yaw, uint16_t throttle);

    bool activeOverride = false;

private:

    bool lastActive;

#define MAX_PITCH 500
#define MAX_ROLL 500
#define MAX_THROTTLE 500

#define KP_X 0.1
#define KP_Y 0.1
#define KP_Z 0.1
#define KP_YAW 0.1

#define KD_X 0.3
#define KD_Y 0.3

#define KI_Z 0.3
#define KI_YAW 0.3

#define ROLL_PD_PWM_FACTOR 1000
#define PITCH_PD_PWM_FACTOR 1000
#define YAW_PI_PWM_FACTOR 1000
#define THROTTLE_PI_PWM_FACTOR 1000

    float lastErrX;
    float lastErrY;

    uint32_t lastYawTime;
    uint32_t lastThrottleTime;

    float yawIntegral;
    float throttleIntegral;

    LoggerLite *logger;

#define AUTOPILOT_LOGGER_TAG "Autopilot"

#define ACTIVE_PWM 1900
#define ACTIVE_PWM_TOLERANCE 100
#define ACTIVE_PIN 10

#define PITCH_PWM_PIN 5
#define ROLL_PWM_PIN 6
#define YAW_PWM_PIN 7
#define THROTTLE_PWM_PIN 8

    Servo pitchPWM;
    Servo yawPWM;
    Servo rollPWM;
    Servo throttlePWM;

    static void onRising();

    static void onFalling();

};


#endif //QUADCOPTER_TESTING_AUTOPILOT_H
