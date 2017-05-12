#include <Arduino.h>
#include "Instruments.h"

SerialLogger logger;
Instruments instruments;

float pos[3];
float yaw;


void setup() {
    logger = SerialLogger(11);
    Serial.begin(19200);
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