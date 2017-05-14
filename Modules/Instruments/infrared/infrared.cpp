// x direction  ............. A0
// y direction  ............. A1
// z direction  ............. A2



#include <stdio.h>

#include "infrared.h"
#include <math.h>

#define PI 3.14159265

using namespace std;

infrared::infrared() {
//	Serial.println("starting. . .");
}

infrared::infrared(SerialLogger *logger) {
    this->logger = logger;
}

void infrared::Calculate() {

    // TODO: Test this filter method and add a better filter if necessary (which it probably is).
    // change condition

    int size = 25;
    float sensorInput[25] = {0};
    float hold;
    int pass;
    float mean[3] = {0};
    static const uint8_t analog_pins[] = {A0, A1, A2};


    for (int infrared = 0; infrared < 3; infrared++) {

        for (int n = 0; n < size; n++) {
            sensorInput[n] = analogRead(analog_pins[infrared]);
        }   //Body Frame x direction infrared sensor

        for (pass = 1; pass < 25; pass++) {
            for (int i = 0; i < size - 1; i++) {
                if (sensorInput[i] > sensorInput[i + 1]) {
                    hold = sensorInput[i];
                    sensorInput[i] = sensorInput[i + 1];
                    sensorInput[i + 1] = hold;
                }
            }
        }
        mean[infrared] = sensorInput[12];
    }


    String logData = "x=" + String(mean[0])
                     + ", y=" + String(mean[1])
                     + ", z=" + String(mean[2]);
    logger->log("Sensors", logData);


    float sensorValue[3];
    // TODO: Individual sensor calibration.
    // To relate the sensor analogue value to distance
    sensorValue[0] = 1271938.7 * pow(mean[0],
                                     -1.7875687);                                        // another set of calculation could be given by
    sensorValue[1] = 176468.59 * pow(mean[1], -1.4945045);
    sensorValue[2] = 2e6 * pow(mean[2] + 5, -2) + 0.05;
    //Serial.print(j);
    //Serial.print("_Absolute Value:");  //debug
    // Serial.println(sensorValue[0]);

    // Distance = 12348.85 * ADC^{-1.05} - 4



    x = sensorValue[0] * cos(abs(pitch_i) * PI / 180.0) *
        abs(cos(abs(yaw_i) * PI / 180.0));     //correct the x coordinate of the quadcopter
    //from pitch, note that yaw_i must be within -90 to 90 degree
    //to give correct value
    y = sensorValue[1] * cos(abs(roll_i) * PI / 180.0) *
        abs(cos(abs(yaw_i) * PI / 180.0));       //correct the y coordinate of the quadcopter from roll
    z = sensorValue[2] * cos(abs(pitch_i) * PI / 180.0) *
        abs(cos(abs(roll_i) * PI / 180.0));    //correct the z coordinate of the quadcopter from yaw,
    //note that pitch_i, roll_i ranges from -90 degree to 90 degree
}


infrared::~infrared() {
//	Serial.println("ending. . .");
}
