//
// Created by Elijah on 16/02/2017.
//

#include "Autopilot.h"

Autopilot::Autopilot() {}

Autopilot::Autopilot(Logger logger) {
    this->logger = logger;
    maxPitch = 30;
    maxRoll = 30;
    Kp = 0.3;
    Kd = 0.1;
}

void Autopilot::calculate(float *target, float *location) {

}