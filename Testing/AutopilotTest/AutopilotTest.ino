#include <Arduino.h>
#include <Autopilot.h>

volatile unsigned long lastPWMTime;
volatile uint16_t pwmValue;
volatile bool autopilotActive;

SerialLogger logger;
Autopilot autopilot;

float tar[] = {0.0, 0.0, 0.0};
float pos[] = {1.0, 1.0, 1.0};

void setup() {
    Serial.begin(9600);

    logger.sync = false;
    autopilot = Autopilot(&logger);

    autopilot.activeOverride = true;

    digitalWrite(10, HIGH);

}

void loop() {
    autopilot.run(tar, pos, 0, 0);
    if (pos[0] + pos[1] + pos[2] > -3) {
        pos[0] -= 0.1;
        pos[1] -= 0.1;
        pos[2] -= 0.1;
    }
    delay(1000);
//    Serial.print("x: "); Serial.print(pos[0]);
//    Serial.print("y: "); Serial.print(pos[1]);
//    Serial.print("z: "); Serial.println(pos[2]);
}