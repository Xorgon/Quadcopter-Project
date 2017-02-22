#include <Arduino.h>
#include <Logger.h>
#include <SD.h>

Logger logger;
const int sdChipSelect = 10;

void setup() {
    Serial.begin(9600);
    //while (!Serial);
    Serial.println("Started.");
    logger = Logger(10);
}

void loop() {
    logger.log("Test", "1 POOP FARTS");
    logger.log("Farts", "A different length string");
    logger.log("Test", "2 POOP FARTS");
    logger.log("Test", "3 POOP FARTS");
    logger.log("Test", "4 POOP FARTS");
    logger.log("Test", "5 POOP FARTS");
    logger.log("Test", "6 POOP FARTS");
    logger.log("Test", "7 POOP FARTS");
    logger.log("Test", "8 POOP FARTS");
    logger.log("Test", "9 POOP FARTS");
    logger.logFile.flush();
}