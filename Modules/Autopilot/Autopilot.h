//
// Created by Elijah on 16/02/2017.
//

#ifndef QUADCOPTER_TESTING_AUTOPILOT_H
#define QUADCOPTER_TESTING_AUTOPILOT_H

#include <Arduino.h>
#include <Logger.h>
#include <math.h>
#include "Servo.h"

// These variables must be created in the main control file.
extern volatile unsigned long lastPWMTime;
extern volatile uint16_t pwmValue;
extern volatile bool autopilotActive;

class Autopilot {
public:
    Autopilot();

    Autopilot(Logger *logger);

    void run(float tar[], float loc[], float yawTar, float yaw);

    uint16_t calculatePitch(float errX); // PD
    uint16_t calculateRoll(float errY); // PD
    uint16_t calculateYaw(float errYaw); // PI
    uint16_t calculateThrottle(float errZ); // PI

    void sendPWM(uint16_t pitch, uint16_t roll, uint16_t yaw, uint16_t throttle);

private:

    bool lastActive;

#define maxPitch 30
#define maxRoll 30
#define maxThrottle 100

#define KP_X 0.1
#define KP_Y 0.1
#define KP_Z 0.1
#define KP_YAW 0.1

#define KD_X 0.3
#define KD_Y 0.3

#define KI_Z 0.3
#define KI_YAW 0.3

#define ROLL_PD_PWM_FACTOR 300
#define PITCH_PD_PWM_FACTOR 300
#define YAW_PI_PWM_FACTOR 300
#define THROTTLE_PI_PWM_FACTOR 300

    float lastErrX;
    float lastErrY;

    uint32_t lastYawTime;
    uint32_t lastThrottleTime;

    float yawIntegral;
    float throttleIntegral;

    Logger *logger;

#define AUTOPILOT_LOGGER_TAG "Autopilot"

#define ACTIVE_PWM 1900
#define ACTIVE_PWM_TOLERANCE 100
#define ACTIVE_PIN 10

#define PITCH_PWM_PIN
#define ROLL_PWM_PIN
#define YAW_PWM_PIN
#define THROTTLE_PWM_PIN

    Servo pitchPWM;
    Servo yawPWM;
    Servo rollPWM;
    Servo throttlePWM;

    static void onRising();

    static void onFalling();

};


#endif //QUADCOPTER_TESTING_AUTOPILOT_H
