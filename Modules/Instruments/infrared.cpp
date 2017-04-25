// x direction  ............. A0
// y direction  ............. A1
// z direction  ............. A2



#include <stdio.h>

#include "infrared.h"
#include <math.h>

#define PI 3.14159265

using namespace std;

infrared::infrared() {
	Serial.println("starting. . .");
}

void infrared::Calculate() {


	int mean[3] = { 0 }; 
	long sensorsum[3] = { 0 };
	float sensorValue[3] = { 0 };

  
        do{
            for (int n = 0; n < 3; n++){
            sensorValue[0] = analogRead(A0);   //Body Frame x direction infrared sensor
            sensorsum[0] = sensorsum[0] + sensorValue[0];
            }
            mean[0] = sensorsum[0]/3;
            sensorsum[0] = 0;        //clear and run
            //Serial.print("mean x:");  //debug
            //Serial.println(mean[0]);
            

        }while(abs(sensorValue[0] - mean[0]) / mean[0] > 10);


        do{
            for (int n = 0; n < 3; n++){
            sensorValue[1] = analogRead(A1);   //Body Frame y direction infrared sensor
            sensorsum[1] = sensorsum[1] + sensorValue[1];
            }
            mean[1] = sensorsum[1]/3;
            sensorsum[1] = 0;        //clear and run

        }while(abs(sensorValue[1] - mean[1]) / mean[1] > 10);


        do{
            for (int n = 0; n < 3; n++){
            sensorValue[2] = analogRead(A2);   //Body Frame z direction infrared sensor
            sensorsum[2] = sensorsum[2] + sensorValue[2];
            }
            mean[2] = sensorsum[2]/3;
            sensorsum[2] = 0;        //clear and run
            

        }while(abs(sensorValue[2] - mean[2]) / mean[2] > 10);






	for (int j = 0; j < 3; j++) {															// To relate the sensor analogue value to distance
		sensorValue[j] = 28000 * pow(mean[j], -1.58);										// another set of calculation could be given by
        //Serial.print(j);
        //Serial.print("_Absolute Value:");  //debug
        //Serial.println(sensorValue[j]);
	}																						// Distance = 12348.85 * ADC^{-1.05} - 4
																													


	x = sensorValue[0] * cos(abs(pitch_i) * PI / 180.0) * cos(abs(yaw_i) * PI / 180.0);     //correct the x coordinate of the quadcopter 
																							//from pitch, note that yaw_i must be within -90 to 90 degree
																							//to give correct value
	y = sensorValue[1] * cos(abs(roll_i) * PI / 180.0) * cos(abs(yaw_i) * PI / 180.0);	   //correct the y coordinate of the quadcopter from roll
	z = sensorValue[2] * cos(abs(pitch_i) * PI / 180.0) * cos(abs(roll_i) * PI / 180.0);	//correct the z coordinate of the quadcopter from yaw, 
																							//note that pitch_i, roll_i ranges from -90 degree to 90 degree
}



infrared::~infrared() {
	Serial.println("ending. . .");
}
