//
// Created by Elijah on 16/02/2017.
//

// TODO: Yaw PI controller.
// TODO: Change to output a proportional PWM rather than angles.
// TODO: Change throttle to use read pitch, roll, rather than using the intended.

#include "Autopilot.h"

Autopilot::Autopilot() {}

Autopilot::Autopilot(Logger *logger) {
    this->logger = logger;

    lastErrX = 999;
    lastErrY = 999;
    lastErrZ = 999;

    pitchPWM.attach(5);
    rollPWM.attach(6);
    yawPWM.attach(7);
    throttlePWM.attach(8);

    attachInterrupt(0, onRising, RISING);

    autopilotActive = false;
    lastActive = false;

    pinMode(ACTIVE_PIN, OUTPUT);
    digitalWrite(ACTIVE_PIN, LOW);
}

void Autopilot::run() {
    if (lastActive != autopilotActive) {
        lastActive = autopilotActive;
        if (autopilotActive) {
            logger->log(AUTOPILOT_LOGGER_TAG, "Autopilot activated.");
        } else {
            logger->log(AUTOPILOT_LOGGER_TAG, "Autopilot deactivated.");
        }
    }
}

float *Autopilot::calculate(float *tar, float *loc) {
    float errX = tar[0] - loc[0];
    float errY = tar[1] - loc[1];
    float errZ = tar[2] - loc[2];

    // Check if there has been no previous value of error, 999 is arbitrary value set at initialization.
    if (lastErrX == 999) {
        lastErrX = errX;
    }
    if (lastErrY == 999) {
        lastErrY = errY;
    }
    if (lastErrZ == 999) {
        lastErrZ = errZ;
    }

    //TODO: Correctly define the coordinate system in relation to the aircraft.
    float pdOutX = KP_X * errX + KD_X * (errX - lastErrX);
    float pdOutY = KP_Y * errY + KD_Y * (errY - lastErrY);
    float pdOutZ = KP_Z * errZ + KD_Z * (errZ - lastErrZ);

    // Sideways acceleration will be proportional to the sin of the pitch/roll angle.
    // pdOut gives the acceleration, so asin is used to make the angle correctly proportional.
    float pitch = 1500 + asin(pdOutX); //TODO: Make a proportionality factor to give correct degrees.
    float roll = 1500 + asin(pdOutY);

    if (pitch > maxPitch) { pitch = maxPitch; }
    if (roll > maxRoll) { roll = maxRoll; }

    // Throttle is proportional to lift/(cos(pitch)*cos(roll)).
    float throttle = pdOutZ / (cos(pitch) * cos(roll)); //TODO: Make a proportionality factor.

    if (throttle > maxThrottle) { throttle = maxThrottle; }

    return; // {pitch, roll, throttle}; TODO: Outputs.
}


//TODO: Fix these interrupt call functions so that they actually work.
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