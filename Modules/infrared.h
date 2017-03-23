
#ifndef INFRARED_H
#define INFRARED_H


//basic method to use this class
//  infrared x;
//	setAngle(pitch, roll, yaw);
//  Calculate();
//  float x= GetDistanceX();
//	float y= GetDistanceY();
//	float z= GetDistanceZ();
//  ~infrared x;                //to save memories
//	I hope it works :)

#include "infrared.cpp"
#include <Adruino.h>
using namespace std;

class infrared {

public:
	infrared();
	void setAngle(int16_t pitch, int16_t roll, int16_t yaw) { pitch_i = pitch/10.0; roll_i = roll/10.0; yaw_i = yaw; }
	void Calculate();
	void GetDistanceX() { return x; }
	void GetDistanceY() { return y; }
	void GetDistanceZ() { return z; }
	~infrared();



private:
	int16_t pitch_i;   
	int16_t roll_i;
	int16_t yaw_i;
	float x;
	float y;
	float z;


};




#endif 