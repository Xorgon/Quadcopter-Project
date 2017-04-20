/*
 * A sketch to log all Serial data to an SD card.
 */

#include <Arduino.h>
#include "Logger.h"

Logger logger;

void setup() {
    Serial.begin(9600);
    logger = Logger(4);
}

void loop() {
    if (Serial.available()) {
        String toWrite = "";
        while (Serial.available()) {
            toWrite.concat(char(Serial.read()));
        }
        logger.log(toWrite);
        Serial.print("/"); // To acknowledge receipt.
    }
    logger.checkFlush();
}