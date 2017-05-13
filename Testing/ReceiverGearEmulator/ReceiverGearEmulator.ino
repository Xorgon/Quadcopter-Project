#include <Arduino.h>
#include <Servo.h>

Servo out;
Servo out2;

Servo yaw;

void setup() {
    out.attach(10);
    out.writeMicroseconds(1900);
    out2.attach(11);
    out2.writeMicroseconds(1200);
    yaw.attach(6);
    yaw.writeMicroseconds(1550);
}

void loop() {

}