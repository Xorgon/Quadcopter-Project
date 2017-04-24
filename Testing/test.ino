#include <infrared.h>


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  infrared sensor;
  sensor.setAngle(10, 20, 10);
  sensor.Calculate();
  float x= sensor.GetDistanceX();
  float y= sensor.GetDistanceY();
  float z= sensor.GetDistanceZ();
  Serial.println("begin");
  Serial.print("x:");
  Serial.println(x);
  Serial.print("y:");
  Serial.println(y);
  Serial.print("z:");
  Serial.println(z);
}
