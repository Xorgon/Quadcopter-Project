#include <Arduino.h>
#include <SerialLogger.h>
#include <Grabber.h>

SerialLogger logger;
Grabber grabber;

void setup() {
    Serial.begin(9600);
    logger = SerialLogger(11);
    logger.sync = false;
    grabber = Grabber(7, &logger, 3000);
    grabber.release();
}

void loop() {
    float pos[] = {1.0, 1.0, 1.0};
    float tar[] = {0.0, 0.0, 0.0};
    grabber.run(pos, tar);
}