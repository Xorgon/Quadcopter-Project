#include <Arduino.h>
//#include <SerialLogger.h>
//#include <Grabber.h>
#include "Servo.h"


//SerialLogger logger;
//Grabber grabber;

Servo servo;
int pos;

void setup() {
//    logger = new SerialLogger();
//    grabber = new Grabber(10, &logger);
    servo = Servo();
    servo.attach(10);
    servo.write(0);
    pos = 0;
}

void loop() {

    moveTo(90);
    delay(1000);
    moveTo(0);
    while(1);
}

void moveTo(int loc) {
    if (pos == loc) {
        return;
    }
    if (pos < loc) {
        for (pos; pos < loc; pos++) {
            servo.write(pos);
            delay(5);
        }
    } else {
        for (pos; pos > loc; pos--) {
            servo.write(pos);
            delay(5);
        }
    }
}