//
// Created by Elijah on 16/02/2017.
//

#include "Autopilot.h"

Autopilot::Autopilot() {}

/**
 * Initializes the Autopilot object.
 * @param logger Pointer to a SerialLogger.
 */
Autopilot::Autopilot(SerialLogger *logger) {
    this->logger = logger;

    resetPID();

    pitchPWM.attach(PITCH_PWM_PIN);
    rollPWM.attach(ROLL_PWM_PIN);
//    yawPWM.attach(YAW_PWM_PIN);
    throttlePWM.attach(THROTTLE_PWM_PIN);

    autopilotActive = false;
    lastActive = false;

    pinMode(ACTIVE_PIN, OUTPUT);
    pinMode(ACTIVE_LED_PIN, OUTPUT);
    digitalWrite(ACTIVE_PIN, LOW);
    digitalWrite(ACTIVE_LED_PIN, LOW);

    pinMode(A5, INPUT_PULLUP);
}

/**
 * Runs the autopilot loop code. First checking whether it is active, then running the PIDs, then sending the PWM signal
 * to the flight controller.
 * @param tar Target location vector {x, y, z} (meters).
 * @param loc Current location vector {x, y, z} (meters).
 * @param yawTar Yaw target (degrees).
 * @param yaw Current yaw (degrees).
 */
void Autopilot::run(float *tar, float *loc, float yawTar, float yaw) {

    if (digitalRead(A5) == LOW || activeOverride){
        digitalWrite(ACTIVE_PIN, HIGH);
        digitalWrite(ACTIVE_LED_PIN, HIGH);
        autopilotActive = true;
    } else {
        digitalWrite(ACTIVE_PIN, LOW);
        digitalWrite(ACTIVE_LED_PIN, LOW);
        autopilotActive = false;
    }

    if (lastActive != autopilotActive) {
        lastActive = autopilotActive;
        if (autopilotActive) {
            logger->log(AUTOPILOT_LOGGER_TAG, F("Autopilot activated."));
        } else {
            logger->log(AUTOPILOT_LOGGER_TAG, F("Autopilot deactivated."));
            resetPID();
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
//                calculateYaw(yawTar - yaw),
                calculateThrottle(err[2])
        );
    }
}

/**
 * Calculates the pitch PWM value based on positional error in the x direction.
 * @param errX Error (meters) in the x direction.
 * @return Pitch PWM value.
 */
uint16_t Autopilot::calculatePitch(float errX) {
    if (lastErrX == 999) {
        lastErrX = errX;
    }
    float pdOut = KP_X * errX + KD_X * (errX - lastErrX);

    // Switch reference frame (+ve x is backwards).
    pdOut = -pdOut;

    uint16_t pwmOut = 1500 + roundf(pdOut * PITCH_PD_PWM_FACTOR);

//    String logData = " P: PID=" + String(pdOut)
//                     + ", pwm=" + String(pwmOut);
//    logger->log(AUTOPILOT_LOGGER_TAG, logData);

    lastErrX = errX;

    if (int(pwmOut - 1500) > 0 && int(pwmOut - 1500) > MAX_PITCH) { pwmOut = 1500 + MAX_PITCH; }
    if (int(pwmOut - 1500) < 0 && -(int(pwmOut - 1500)) > MAX_PITCH) { pwmOut = 1500 - MAX_PITCH; }

    return pwmOut;
}

/**
 * Calculates the roll PWM value based on positional error in the y direction.
 * @param errY Error (meters) in the y direction.
 * @return Roll PWM value.
 */
uint16_t Autopilot::calculateRoll(float errY) {
    if (lastErrY == 999) {
        lastErrY = errY;
    }
    float pdOut = KP_Y * errY + KD_Y * (errY - lastErrY);
    // Reference frame is correct (+ve y is towards +ve roll).

    uint16_t pwmOut = 1500 + roundf(pdOut * ROLL_PD_PWM_FACTOR);

//    String logData = " R: PID=" + String(pdOut)
//                     + ", pwm=" + String(pwmOut);
//    logger->log(AUTOPILOT_LOGGER_TAG, logData);

    lastErrY = errY;

    if (int(pwmOut - 1500) > 0 && int(pwmOut - 1500) > MAX_ROLL) { pwmOut = 1500 + MAX_ROLL; }
    if (int(pwmOut - 1500) < 0 && -(int(pwmOut - 1500)) > MAX_ROLL) { pwmOut = 1500 - MAX_ROLL; }

    return pwmOut;
}

/**
 * Calculates the yaw PWM value based on positional error of yaw.
 * @param errYaw Error (degrees) of yaw.
 * @return Yaw PWM value.
 */
uint16_t Autopilot::calculateYaw(float errYaw) {
    uint32_t now = millis();
    if (lastYawTime == 0) {
        lastYawTime = now;
    }
    yawIntegral += (now - lastYawTime) * errYaw;

    float pdOut = KP_YAW * errYaw + KI_YAW * yawIntegral / 1000;

    uint16_t pwmOut = 1500 + roundf(pdOut * YAW_PI_PWM_FACTOR);

    String logData = " Y: PID=" + String(pdOut)
                     + ", pwm=" + String(pwmOut);
    logger->log(AUTOPILOT_LOGGER_TAG, logData);

    lastYawTime = now;

    return pwmOut;
}

/**
 * Calculates the throttle PWM value based on positional error in the z direction.
 * @param errZ Error (meters) in the z direction.
 * @return Throttle PWM value.
 */
uint16_t Autopilot::calculateThrottle(float errZ) {
    if (lastErrZ == 999) {
        lastErrZ = errZ;
    }

    uint32_t now = millis();
    if (lastThrottleTime == 0) {
        lastThrottleTime = now;
    }
    throttleIntegral += (now - lastThrottleTime) * errZ / 1000;

    float pdOut = KP_Z * errZ + KI_Z * throttleIntegral + KD_Z * (errZ - lastErrZ);

    uint16_t pwmOut = THROTTLE_CENTER + roundf(pdOut * THROTTLE_PI_PWM_FACTOR);

    String logData = " T: PID=" + String(pdOut)
                     + ", pwm=" + String(pwmOut);
    logger->log(AUTOPILOT_LOGGER_TAG, logData);

    lastErrZ = errZ;
    lastThrottleTime = now;

    if (int(pwmOut - THROTTLE_CENTER) > 0 && int(pwmOut - THROTTLE_CENTER) > MAX_THROTTLE) {
        pwmOut = THROTTLE_CENTER + MAX_THROTTLE;
    }
    if (int(pwmOut - THROTTLE_CENTER) < 0 && -(int(pwmOut - THROTTLE_CENTER)) > MIN_THROTTLE) {
        pwmOut = THROTTLE_CENTER - MIN_THROTTLE;
    }

    return pwmOut;
}

#define PWM_CORRECTION_NUMBER -2

/**
 * Sends pwm signals to the control Servo outputs.
 *
 * @param pitch Pitch PWM value.
 * @param roll Roll PWM value.
 * @param yaw Yaw PWM value.
 * @param throttle Throttle PWM value.
 */
void Autopilot::sendPWM(uint16_t pitch, uint16_t roll, uint16_t yaw, uint16_t throttle) {
    pitchPWM.write(pitch + PWM_CORRECTION_NUMBER);
    rollPWM.write(roll + PWM_CORRECTION_NUMBER);
//    yawPWM.write(yaw + PWM_CORRECTION_NUMBER); // Removed for compiling.
    throttlePWM.write(throttle + PWM_CORRECTION_NUMBER);
}

/**
 * Sends pwm signals to the control Servo outputs except for yaw.
 *
 * @param pitch Pitch PWM value.
 * @param roll Roll PWM value.
 * @param throttle Throttle PWM value.
 */
void Autopilot::sendPWM(uint16_t pitch, uint16_t roll, uint16_t throttle) {
    pitchPWM.write(pitch + PWM_CORRECTION_NUMBER);
    rollPWM.write(roll + PWM_CORRECTION_NUMBER);
    throttlePWM.write(throttle + PWM_CORRECTION_NUMBER);
}

/**
 * Resets the PID loops.
 */
void Autopilot::resetPID() {
    lastErrX = 999;
    lastErrY = 999;
    lastErrZ = 999;

    yawIntegral = 0;
    throttleIntegral = 0;

    lastYawTime = 0;
    lastThrottleTime = 0;
}