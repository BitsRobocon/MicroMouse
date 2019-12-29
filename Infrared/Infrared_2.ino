/*
 * Name :Infrared_2.ino
 * Author : Hardik Jain (nepython)
 * Last Update : 29/12/19  
 */

#include <SharpIR.h>

#define front_ir A0
#define left_ir A1
#define right_ir A2
#define model 430                         //Model of SHARP IR we used
// ir: the pin where your sensor is attached
// model: an int that determines your sensor:  1080 for GP2Y0A21Y
//                                            20150 for GP2Y0A02Y
//                                            (working distance range according to the datasheets)

void setup() {
  Serial.begin(9600);
}

void loop() {
  delay(2000);
  //Serial.print("Front: ");  
  //Serial.println(f());
  //Serial.print("Left: ");  
  //Serial.print(l());
  //Serial.print("Right: ");  
  //Serial.println(r());

}

float f()
{
  SharpIR SharpIR(front_ir, model);  
  return SharpIR.distance();
}

float l()
{
  SharpIR SharpIR(left_ir, model);  
  return (SharpIR.distance() + 0.45); //accounting for difference in values
}

float r()
{
  SharpIR SharpIR(right_ir, model);  
  return SharpIR.distance();
}
