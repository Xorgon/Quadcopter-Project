#include <Arduino.h>
#include <Logger.h>

Logger logger;
const int sdChipSelect = 10;

void setup() {
    Serial.begin(9600);
    //while (!Serial);
    Serial.println("Started.");
    logger = Logger(10);
}

void loop() {
    logger.log("Test", "POOP FARTS");
    delay(1000);
}