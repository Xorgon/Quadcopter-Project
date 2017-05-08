
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

#include <Arduino.h>

using namespace std;

class infrared {

public:
    infrared();

    infrared(SerialLogger *logger);

    void setAngle(int16_t pitch, int16_t roll, int16_t yaw) {
        pitch_i = pitch;
        roll_i = roll;
        yaw_i = yaw;
    }

    void Calculate();

    float GetDistanceX() { return x; }

    float GetDistanceY() { return y; }

    float GetDistanceZ() { return z; }

    ~infrared();


private:
    SerialLogger *logger;

    int16_t pitch_i;
    int16_t roll_i;
    int16_t yaw_i;
    float x;
    float y;
    float z;


};


#endif 3
