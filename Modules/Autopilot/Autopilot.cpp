//
// Created by Elijah on 16/02/2017.
//

#include "Autopilot.h"

Autopilot::Autopilot() {}

Autopilot::Autopilot(LoggerLite *logger) {
    this->logger = logger;

    lastErrX = 999;
    lastErrY = 999;

    yawIntegral = 0;
    throttleIntegral = 0;

    lastYawTime = 0;
    lastThrottleTime = 0;

    pitchPWM.attach(PITCH_PWM_PIN);
    rollPWM.attach(ROLL_PWM_PIN);
    yawPWM.attach(YAW_PWM_PIN);
    throttlePWM.attach(THROTTLE_PWM_PIN);

    attachInterrupt(0, onRising, RISING);

    autopilotActive = false;
    lastActive = false;

    pinMode(ACTIVE_PIN, OUTPUT);
    digitalWrite(ACTIVE_PIN, LOW);
}

void Autopilot::run(float *tar, float *loc, float yawTar, float yaw) {
    if (lastActive != autopilotActive) {
        lastActive = autopilotActive;
        if (autopilotActive) {
            logger->log(AUTOPILOT_LOGGER_TAG, "Autopilot activated.");
        } else {
            logger->log(AUTOPILOT_LOGGER_TAG, "Autopilot deactivated.");
        }
    }

    if (autopilotActive || activeOverride) {
        float err[3];
        err[0] = tar[0] - loc[0];
        err[1] = tar[1] - loc[1];
        err[2] = tar[2] - loc[2];

        sendPWM(
                calculatePitch(err[0]),
                calculateRoll(err[1]),
                calculateYaw(yawTar - yaw),
                calculateThrottle(err[2])
        );
    }
}

uint16_t Autopilot::calculatePitch(float errX) {
    if (lastErrX == 999) {
        lastErrX = errX;
    }
    float pdOut = KP_X * errX + KD_X * (errX - lastErrX);

    uint16_t pwmOut = 1500 + roundf(pdOut * PITCH_PD_PWM_FACTOR);

    lastErrX = errX;

    if (int(pwmOut - 1500) > 0 && int(pwmOut - 1500) > MAX_PITCH) { pwmOut = 1500 + MAX_PITCH; }
    if (int(pwmOut - 1500) < 0 && -(int(pwmOut - 1500)) > MAX_PITCH) { pwmOut = 1500 - MAX_PITCH; }

    return pwmOut;
}

uint16_t Autopilot::calculateRoll(float errY) {
    if (lastErrY == 999) {
        lastErrY = errY;
    }
    float pdOut = KP_Y * errY + KD_Y * (errY - lastErrY);

    uint16_t pwmOut = 1500 + roundf(pdOut * ROLL_PD_PWM_FACTOR);

    lastErrY = errY;

    if (int(pwmOut - 1500) > 0 && int(pwmOut - 1500) > MAX_ROLL) { pwmOut = 1500 + MAX_ROLL; }
    if (int(pwmOut - 1500) < 0 && -(int(pwmOut - 1500)) > MAX_ROLL) { pwmOut = 1500 - MAX_ROLL; }

    return pwmOut;
}

uint16_t Autopilot::calculateYaw(float errYaw) {
    uint32_t now = millis();
    if (lastYawTime == 0) {
        lastYawTime = now;
    }
    yawIntegral += (now - lastYawTime) * errYaw;

    float pdOut = KP_YAW * errYaw + KI_YAW * yawIntegral / 1000;

    uint16_t pwmOut = 1500 + roundf(pdOut * YAW_PI_PWM_FACTOR);

    lastYawTime = now;

    return pwmOut;
}

// TODO: Check throttle active range.
uint16_t Autopilot::calculateThrottle(float errZ) {
    uint32_t now = millis();
    if (lastThrottleTime == 0) {
        lastThrottleTime = now;
    }
    throttleIntegral += (now - lastThrottleTime) * errZ / 1000;

    float pdOut = KP_Z * errZ + KI_Z * throttleIntegral;

    uint16_t pwmOut = 1500 + roundf(pdOut * THROTTLE_PI_PWM_FACTOR);

    lastThrottleTime = now;

    if (int(pwmOut - 1500) > 0 && int(pwmOut - 1500) > MAX_THROTTLE) { pwmOut = 1500 + MAX_THROTTLE; }
    if (int(pwmOut - 1500) < 0 && -(int(pwmOut - 1500)) > MAX_THROTTLE) { pwmOut = 1500 - MAX_THROTTLE; }

    return pwmOut;
}

#define PWM_CORRECTION_NUMBER -2

void Autopilot::sendPWM(uint16_t pitch, uint16_t roll, uint16_t yaw, uint16_t throttle) {
    pitchPWM.writeMicroseconds(pitch + PWM_CORRECTION_NUMBER);
    rollPWM.writeMicroseconds(roll + PWM_CORRECTION_NUMBER);
    yawPWM.writeMicroseconds(yaw + PWM_CORRECTION_NUMBER);
    throttlePWM.writeMicroseconds(throttle + PWM_CORRECTION_NUMBER);
}


static void Autopilot::onRising() {
    attachInterrupt(0, onFalling, FALLING);
    lastPWMTime = micros();
}

static void Autopilot::onFalling() {
    attachInterrupt(0, onRising, RISING);
    pwmValue = micros() - lastPWMTime;
    if (pwmValue > (ACTIVE_PWM - ACTIVE_PWM_TOLERANCE)
        and pwmValue < (ACTIVE_PWM + ACTIVE_PWM_TOLERANCE)) {
        digitalWrite(ACTIVE_PIN, HIGH);
        autopilotActive = true;
    } else {
        digitalWrite(ACTIVE_PIN, LOW);
        autopilotActive = false;
    }
}