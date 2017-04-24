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
	int sensorValue[3] = { 0 };
	sensorValue[0] = analogRead(A0);   //Body Frame x direction infrared sensor
	sensorValue[1] = analogRead(A1);   //....y.....
	sensorValue[2] = analogRead(A2);   //....z.....

	for (int n = 0; n < 3; n++) {
		for (int i = 0; i < 3; i++) {
			if (abs(sensorValue[n] - mean[n]) / mean[n] < 10) {
				sensorsum[n] = sensorsum[n] + sensorValue[n];
				mean[n] = sensorsum[n] / 3;
			}
		}
	}

	for (int j = 0; j < 3; j++) {															// To relate the sensor analogue value to distance
		sensorValue[j] = 28000 * pow(mean[j], -1.58);										// another set of calculation could be given by 
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
