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

void infrared::Calculate() {

    // TODO: Test this filter method and add a better filter if necessary (which it probably is).
    // change condition
    float mean[3] = {0};
    float sensorsum = 0;
    float sensorValue[3] = {0};
    float sensorInput[25]={0};
    float max;
    float min;


    //do {
        max=0.0;
        min=10000.0;
        
        for (int n = 0; n < 25; n++) {sensorInput[n] = analogRead(A0);
        delay(1);}   //Body Frame x direction infrared sensor
        
        for(int n=0; n<25;n++){
        if(sensorInput[n]>max) max=sensorInput[n];
        if(sensorInput[n]<min) min=sensorInput[n];
           sensorsum = sensorsum + sensorInput[n];
        }
        
        
        mean[0] = (sensorsum-max-min) / 3.0;
        sensorsum = 0.0;        //clear and run
        //Serial.print("mean x:");  //debug
        //Serial.println(mean[0]);


    //} while ((abs(analogRead(A0) - mean[0]) / mean[0]) > 0.0005);


    do {
        max=0.0;
        min=10000.0;
        
        for (int n = 0; n < 5; n++) { sensorInput[n] = analogRead(A1);}   //Body Frame y direction infrared sensor
        
        for(int n=0; n<5;n++){
        if(sensorInput[n]>max) max=sensorInput[n];
        if(sensorInput[n]<min) min=sensorInput[n];
           sensorsum = sensorsum + sensorInput[n];
        }
        
        mean[1] = (sensorsum-max-min) / 3.0;
        sensorsum = 0.0;        //clear and run

    } while (abs(analogRead(A1) - mean[1]) / mean[1] > 0.0005);


    do {
        max=0.0;
        min=10000.0;
        for (int n = 0; n < 5; n++) { sensorInput[n] = analogRead(A2);}  //Body Frame z direction infrared sensor
        
        for(int n=0; n<5;n++){
        if(sensorInput[n]>max) max=sensorInput[n];
        if(sensorInput[n]<min) min=sensorInput[n];
           sensorsum = sensorsum + sensorInput[n];
        }
        
        mean[2] = (sensorsum-max-min) / 3.0;
        sensorsum = 0.0;        //clear and run


    } while ((abs(analogRead(A2) - mean[2]) / mean[2]) > 0.0005);





    // TODO: Individual sensor calibration.
    // To relate the sensor analogue value to distance
    sensorValue[0] = 1271938.7 * pow(mean[0], -1.7875687);                                        // another set of calculation could be given by
    sensorValue[1] = 176468.59 * pow(mean[1], -1.4945045);
    sensorValue[2] = 783622.75 * pow(mean[2], -1.7171497);
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
