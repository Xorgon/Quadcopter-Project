//
// Created by Elijah on 14/02/2017.
//

#include <iostream>
#include <cstring>
#include <Autopilot.h>

int main() {
    Autopilot autopilot = Autopilot();
    float loc[] = {1.0, 1.0, 1.0};
    float tar[] = {0.0, 0.0, 0.0};
    float *out = autopilot.calculate(tar, loc);

    std::cout << *out << std::endl;
    
    return 0;
}
