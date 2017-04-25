#include <infrared.h>


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  infrared sensor;
  sensor.setAngle(0, 300, 0);
  sensor.Calculate();
  
  //float TheoreticalValue = 28000 * pow(analogRead(A2), -1.58);    //debug
  //Serial.print("Theoretical Value: ");   //debug
  //Serial.println(TheoreticalValue);     //debug
  
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
