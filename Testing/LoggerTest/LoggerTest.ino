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
    logger.log("Farm", "1 Chicken");
    logger.log("Farm", "2 Pigs");
    logger.log("Farm", "3 Dogs");
    logger.log("Farm", "4 Sheep");
    logger.log("Farm", "5 Cows");
    logger.log("Farm", "6 Alpacas");
    logger.log("Farm", "7 Goats");
    logger.log("Farm", "8 Turkeys");
    logger.log("Farm", "9 Horses");
    logger.logFile.flush();
}