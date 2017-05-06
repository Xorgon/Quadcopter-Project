#include <Arduino.h>
#include <SerialLogger.h>
#include <Grabber.h>

SerialLogger logger;
Grabber grabber;


void setup() {
    logger = SerialLogger();
    grabber = Grabber(10, &logger);
}

void loop() {
    grabber.release();
    while(1);
}