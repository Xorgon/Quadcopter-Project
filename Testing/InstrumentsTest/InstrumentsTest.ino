#include <Arduino.h>
#include "Instruments.h"

SerialLogger logger;
Instruments instruments;

float pos[3];
float yaw;


void setup() {
    Serial.begin(9600);
    instruments = Instruments(&logger, 8, 9);
    logger.sync = false;
    pos[0] = 1.0;
    pos[1] = 1.0;
    pos[2] = 1.0;
    yaw = 0;
}

void loop() {
    yaw = instruments.setPos(pos);
    delay(500);
}