//
// Created by Elijah on 16/02/2017.
//

#include "Autopilot.h"

Autopilot::Autopilot() {}

Autopilot::Autopilot(Logger logger) {
    this->logger = logger;
    maxPitch = 30;
    maxRoll = 30;
    Kp = {0.3, 0.3, 0.3};
    Kd = {0.1, 0.1, 0.1};

    lastErrX = 999;
    lastErrY = 999;
    lastErrZ = 999;
}

float* Autopilot::calculate(float *tar, float *loc) {
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
    float pdOutX = Kp[0] * errX + Kd[0] * (errX - lastErrX);
    float pdOutY = Kp[1] * errY + Kd[1] * (errY - lastErrY);
    float pdOutZ = Kp[2] * errZ + Kd[2] * (errZ - lastErrZ);

    // Sideways acceleration will be proportional to the sin of the pitch/roll angle.
    // pdOut gives the acceleration, so asin is used to make the angle correctly proportional.
    float pitch = asin(pdOutX); //TODO: Make a proportionality factor to give correct degrees.
    float roll = asin(pdOutY);

    if (pitch > maxPitch) { pitch = maxPitch; }
    if (roll > maxRoll) { roll = maxRoll; }

    // Throttle is proportional to lift/(cos(pitch)*cos(roll)).
    float throttle = pdOutZ / (cos(pitch) * cos(roll)); //TODO: Make a proportionality factor.

    if (throttle > maxThrottle) { throttle = maxThrottle; }

    return {pitch, roll, throttle};
}