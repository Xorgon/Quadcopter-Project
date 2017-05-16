//
// Created by Elijah on 16/02/2017.
//

#include "Autopilot.h"

/**
 * Allows for declaration of an Autopilot variable without initialization.
 */
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

    // Set output pin modes and make sure they start low.
    pinMode(ACTIVE_PIN, OUTPUT);
    pinMode(ACTIVE_LED_PIN, OUTPUT);
    digitalWrite(ACTIVE_PIN, LOW);
    digitalWrite(ACTIVE_LED_PIN, LOW);

    // Set activation pin mode.
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

    // Checks the activation input and activeOverride to see if the autopilot should be active.
    if (digitalRead(A5) == LOW || activeOverride) {
        digitalWrite(ACTIVE_PIN, HIGH);
        digitalWrite(ACTIVE_LED_PIN, HIGH);
        autopilotActive = true;
    } else {
        digitalWrite(ACTIVE_PIN, LOW);
        digitalWrite(ACTIVE_LED_PIN, LOW);
        autopilotActive = false;
    }

    // Checks whether the autopilot status has changed and logs the change.
    if (lastActive != autopilotActive) {
        lastActive = autopilotActive;
        if (autopilotActive) {
            logger->log(AUTOPILOT_LOGGER_TAG, F("Autopilot activated."));
        } else {
            logger->log(AUTOPILOT_LOGGER_TAG, F("Autopilot deactivated."));
            resetPID();
        }
    }

    // Runs PID calculations if the autopilot is active.
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
    // Check if the last error hasn't been set yet.
    if (lastErrX == 999) {
        lastErrX = errX;
    }

    // Calculate PD controller output.
    float pdOut = KP_X * errX + KD_X * (errX - lastErrX);

    // Switch reference frame (+ve x is towards -ve pitch).
    pdOut = -pdOut;

    // Convert PD controller output to PWM output.
    uint16_t pwmOut = 1500 + roundf(pdOut * PITCH_PD_PWM_FACTOR);

    // Disabled to increase loops per second.
//    String logData = " P: PID=" + String(pdOut)
//                     + ", pwm=" + String(pwmOut);
//    logger->log(AUTOPILOT_LOGGER_TAG, logData);

    // Set the last error for the next loop.
    lastErrX = errX;

    // Ensure that the PWM output is within limits.
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
    // Check if the last error hasn't been set yet.
    if (lastErrY == 999) {
        lastErrY = errY;
    }

    // Calculate PD controller output.
    float pdOut = KP_Y * errY + KD_Y * (errY - lastErrY);
    // Reference frame is correct (+ve y is towards +ve roll).

    // Convert PD controller output to PWM output
    uint16_t pwmOut = 1500 + roundf(pdOut * ROLL_PD_PWM_FACTOR);

    // Disabled to increase loops per second.
//    String logData = " R: PID=" + String(pdOut)
//                     + ", pwm=" + String(pwmOut);
//    logger->log(AUTOPILOT_LOGGER_TAG, logData);

    // Set the last error for the next loop.
    lastErrY = errY;

    // Ensure that the PWM output is within limits.
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
    // Store the current milliseconds to reduce calls to millis().
    uint32_t now = millis();

    // Check if the last calculation time hasn't been set yet.
    if (lastYawTime == 0) {
        lastYawTime = now;
    }

    // Calculate the finite step integral.
    yawIntegral += (now - lastYawTime) * errYaw;

    // Calculate PI controller output.
    float piOut = KP_YAW * errYaw + KI_YAW * yawIntegral / 1000;

    // Convert PI controller output to PWM output.
    uint16_t pwmOut = 1500 + roundf(piOut * YAW_PI_PWM_FACTOR);

    // Log calculation data.
    String logData = " Y: PID=" + String(piOut)
                     + ", pwm=" + String(pwmOut);
    logger->log(AUTOPILOT_LOGGER_TAG, logData);

    // Set the last calculation time for the next loop.
    lastYawTime = now;

    return pwmOut;
}

/**
 * Calculates the throttle PWM value based on positional error in the z direction.
 * @param errZ Error (meters) in the z direction.
 * @return Throttle PWM value.
 */
uint16_t Autopilot::calculateThrottle(float errZ) {
    // Check if the last error hasn't been set yet.
    if (lastErrZ == 999) {
        lastErrZ = errZ;
    }

    // Store the current milliseconds to reduce calls to millis().
    uint32_t now = millis();

    // Check if the last calculation time hasn't been set yet.
    if (lastThrottleTime == 0) {
        lastThrottleTime = now;
    }

    // Calculate the finite step integral.
    throttleIntegral += (now - lastThrottleTime) * errZ / 1000;

    // Calculate PID controller output.
    float pidOut = KP_Z * errZ + KI_Z * throttleIntegral + KD_Z * (errZ - lastErrZ);

    // Convert PID controller output to PWM output.
    uint16_t pwmOut = THROTTLE_CENTER + roundf(pidOut * THROTTLE_PI_PWM_FACTOR);

    // Log calculation data.
    String logData = " T: PID=" + String(pidOut)
                     + ", pwm=" + String(pwmOut);
    logger->log(AUTOPILOT_LOGGER_TAG, logData);

    // Set the last error and last calculation time for the next loop.
    lastErrZ = errZ;
    lastThrottleTime = now;

    // Ensure that the PWM output is within limits.
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
    // Reset last errors to the "unset" values.
    lastErrX = 999;
    lastErrY = 999;
    lastErrZ = 999;

    // Reset integrals to zero.
    yawIntegral = 0;
    throttleIntegral = 0;

    // Reset last times to the "unset" values.
    lastYawTime = 0;
    lastThrottleTime = 0;
}