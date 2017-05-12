#include <Arduino.h>
#include <ServoTimer2.h>

ServoTimer2 servo;

void setup() {
    servo.attach(3);
    servo.write(1200);
}

void loop() {
    servo.write(1600);
    delay(100);
    servo.write(1200);
    delay(100);
}