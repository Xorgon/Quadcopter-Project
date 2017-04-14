#include <Arduino.h>

float maxPitch;
float maxRoll;
float maxThrottle;
float Kp[3] = {0.1, 0.1, 0.1};
float Kd[3] = {0.3, 0.3, 0.3};


float lastErrX;
float lastErrY;
float lastErrZ;

uint16_t activePWM = 1900; //TODO: Test activePWM value.
volatile unsigned long lastPWMTime = 0;
volatile uint16_t pwmValue = 0;
uint8_t pwmTolerance = 100;
uint8_t activePin = 10;

void setup() {
    Serial.begin(9600);
    float tar[3] = {1, 1, 1};
    float cur[3] = {0.5, 1, 1};
    calculate(tar, cur);
}

void loop() {
}

float *calculate(float *tar, float *loc) {
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
    float pitch = 1500 + 100 * asin(pdOutX); //TODO: Make a proportionality factor to give correct degrees.
    float roll = 1500 + 100 * asin(pdOutY);

    Serial.println(pitch);
    Serial.println(roll);

    // Throttle is proportional to lift/(cos(pitch)*cos(roll)).
    float throttle = pdOutZ / (cos(pitch) * cos(roll)); //TODO: Make a proportionality factor.

    if (throttle > maxThrottle) { throttle = maxThrottle; }

    return; // {pitch, roll, throttle}; TODO: Outputs.
}