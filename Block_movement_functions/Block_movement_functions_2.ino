
/*
 * Block_movement_fuctions_2.ino
 * Author : Hardik Jain (nepython)
 * Last update :29/12/19
 */
 
# define error_threshold 5                  //To be defined error of threshold
#define RIGHT_MOTOR_ENABLE 6 // enable pin for right motor
#define LEFT_MOTOR_ENABLE 7  // enable pin for left MOTOR
#include <SharpIR.h>
#include<Encoder.h> 
// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

// MOTOR
    #define RIGHT_MOTOR_1 4  // pin 1 for right motor
    #define RIGHT_MOTOR_2 5  // pin 2 for right motor
    #define LEFT_MOTOR_1 9   // pin 1 for left motor
    #define LEFT_MOTOR_2 17  // pin 2 for left motor
    
Encoder myEnc1(19,18);
Encoder myEnc2(20,21);
//   avoid using pins with LEDs attached

//Global Flags 
int a=0;      //Encoder flags
int b=0; 

//int l=0;      //Flags of previous move
//int f=0;
//int r=0;

int L_enable_val=255;   //ENABLE Pins Values
int R_enable_val=255;

void go_forward(long);
void drive_straight(float, float);

#define front_ir A0
#define left_ir A1
#define right_ir A2
#define model 430
// ir: the pin where your sensor is attached
// model: an int that determines your sensor:  1080 for GP2Y0A21Y
//                                            20150 for GP2Y0A02Y
//                                            (working distance range according to the datasheets)


void setup() {
  Serial.begin(9600);
}

void loop() 
{}

void go_forward(long distance) 
{
  distance=((distance*820)/(3*3.141593))*0.96;
  long distance1=abs(myEnc1.read());
  long distance2=abs(myEnc2.read());   
 /* if(l==1)
    distance=distance+abs(myEnc2.read());
    
  if(r==1)
    distance1=abs(myEnc1.read());

  if(f==1)
    distance=distance+(abs(myEnc1.read()))+abs(myEnc1.read()/2);*/
  
  long oldPosition1=(-999);
  long oldPosition2=(-999);
  //long distance=1392;
  long newPosition1=0;
  long newPosition2=0;
  while((a+b)!=2)
  {
    drive_straight(r(),l());
    if(newPosition1<distance*0.5+distance1)
    {
      newPosition1 = abs(myEnc1.read());
      if (newPosition1 != oldPosition1) 
      {
        oldPosition1 = newPosition1;
        Serial.println(newPosition1);
      }
      analogWrite(RIGHT_MOTOR_1,255);
      analogWrite(RIGHT_MOTOR_2,0);
    }
    else if(newPosition1>(distance*0.5+distance1)&&newPosition1<(distance*0.7+distance1))
    {
      analogWrite(RIGHT_MOTOR_1,100);
      analogWrite(RIGHT_MOTOR_2,0); 
      newPosition1 = abs(myEnc1.read());
      if (newPosition1 != oldPosition1) 
      {
        oldPosition1 = newPosition1;
        Serial.println(newPosition1);
      }
    }
    else if(newPosition1>(distance*0.7+distance1)&&newPosition1<(distance+distance1))
    {
      analogWrite(RIGHT_MOTOR_1,60);
      analogWrite(RIGHT_MOTOR_2,0); 
      newPosition1 = abs(myEnc1.read());
      if (newPosition1 != oldPosition1) 
      {
        oldPosition1 = newPosition1;
        Serial.println(newPosition1);
      }
    }
    else
    {
      analogWrite(RIGHT_MOTOR_1,0);
      analogWrite(RIGHT_MOTOR_2,0);
      //Serial.println("a=1");
      a=1;
    }

    if(newPosition2<distance*0.5+distance2)
    {
      newPosition2 = abs(myEnc2.read());
      if (newPosition2 != oldPosition2) 
      {
        oldPosition2 = newPosition2;
        Serial.println(newPosition2);
      }
      analogWrite(LEFT_MOTOR_1,255);
      analogWrite(LEFT_MOTOR_2,0);
    }
    else if(newPosition2>(distance*0.5+distance2)&&newPosition2<(distance*0.7+distance2))
    {
      analogWrite(LEFT_MOTOR_1,100);
      analogWrite(LEFT_MOTOR_2,0); 
      newPosition2 = abs(myEnc2.read());
      if (newPosition2 != oldPosition2) 
      {
        oldPosition2 = newPosition2;
        Serial.println(newPosition2);
      }
    }
    else if(newPosition2>(distance*0.7+distance2)&&newPosition2<(distance+distance2))
    {
      analogWrite(LEFT_MOTOR_1,60);
      analogWrite(LEFT_MOTOR_2,0); 
      newPosition2 = abs(myEnc2.read());
      if (newPosition2 != oldPosition2) 
      {
        oldPosition2 = newPosition2;
        Serial.println(newPosition2);
      }
    }
    else 
    {
      analogWrite(LEFT_MOTOR_1,0);
      analogWrite(LEFT_MOTOR_2,0);
      //Serial.println("b=1");
      b=1;
    }
    if((a+b)==2)
    {
      a=0;
      b=0;
      //L_enable_val=255;
      //R_enable_val=255;
      return;
    }
  }
 }

/******************************************************************************
 void go_backward(long distance,long oldPosition1=-999,long oldPosition2=-999) 
{
  //long distance=1392;
  long newPosition1=0;
  long newPosition2=0;
  if(newPosition1<distance*0.5)
  {
    newPosition1 = abs(myEnc1.read());
    if (newPosition1 != oldPosition1) 
    {
      oldPosition1 = newPosition1;
      Serial.println(newPosition1);
    }
  }
  if(newPosition2<distance*0.5)
  {
    newPosition2 = abs(myEnc2.read());
    if (newPosition2 != oldPosition2) 
    {
      oldPosition2 = newPosition2;
      Serial.println(newPosition2);
    }
  }
  if(newPosition1>(distance*0.5)&&newPosition1<distance*0.7)
  {
    analogWrite(RIGHT_MOTOR_2,100); 
    newPosition1 = myEnc1.read();
    if (newPosition1 != oldPosition1) 
    {
      oldPosition1 = newPosition1;
      Serial.println(newPosition1);
    }
  }
  if(newPosition2>(distance*0.5)&&newPosition2<distance*0.7)
  {
    analogWrite(LEFT_MOTOR_2,100); 
    newPosition2 = abs(myEnc2.read());
    if (newPosition2 != oldPosition2) 
    {
      oldPosition2 = newPosition2;
      Serial.println(newPosition2);
    }
  }
  if(newPosition1>(distance*0.7)&&newPosition1<distance*0.85)
  {
    analogWrite(RIGHT_MOTOR_2,70); 
    newPosition1 = abs(myEnc1.read());
    if (newPosition1 != oldPosition1) 
    {
      oldPosition1 = newPosition1;
      Serial.println(newPosition1);
    }
  }
  if(newPosition2>(distance*0.7)&&newPosition2<distance*0.85)
  {
    analogWrite(LEFT_MOTOR_2,70); 
    newPosition2 = abs(myEnc2.read());
    if (newPosition2 != oldPosition2) 
    {
      oldPosition2 = newPosition2;
      Serial.println(newPosition2);
    }
  }
  if(newPosition1>distance*0.85)
  {
    analogWrite(RIGHT_MOTOR_2,0);
    a=1;
  }
  if(newPosition2>distance*0.85)      //NOT working properly, needs to be tuned
  {
    analogWrite(LEFT_MOTOR_2,0);
    b=1;
  }
  go_backward(1392,oldPosition1,oldPosition2);
}

*******************************************************************************************/
void turn_right()
{
    // turn right and go one block ahead
    // need to do some axis checking with maze and edit
    
    // Distance between centres of the 2 wheels~~101mm
    // Quarter circle traversed by each wheel=pi*50.5/2=79.32mm
    
    int flag1=0;
    int flag2=0;
    long distance=360+abs(myEnc2.read());
    long oldPosition1  = -999;
    long oldPosition2  = -999;
    long newPosition1=0;
    long newPosition2=0;
    
        if (newPosition1 != oldPosition1) 
        {
          oldPosition1 = newPosition1;
          Serial.println(newPosition1);
        }
        if (newPosition2 != oldPosition2) 
        {
          oldPosition2 = newPosition2;
          Serial.println(newPosition2);
        }
    while(flag1+flag2<2)
    {
      if(flag1!=1)
      {
        analogWrite(RIGHT_MOTOR_2,255); 
        analogWrite(RIGHT_MOTOR_1,0);
      }
      if(flag2!=1)
      { 
        analogWrite(LEFT_MOTOR_2,0); 
        analogWrite(LEFT_MOTOR_1,255);
      }
      if(newPosition2<distance)
      {
        newPosition2 = abs(myEnc2.read());
        if (newPosition2 != oldPosition2) 
        {
          oldPosition2 = newPosition2;
          Serial.println(newPosition2);
        }
      }
      else
        {
          flag2=1;
          analogWrite(LEFT_MOTOR_2,0); 
          analogWrite(LEFT_MOTOR_1,0);
        }
      if(newPosition1<distance)
      {
        newPosition1 = abs(myEnc2.read());
        if (newPosition1 != oldPosition1) 
        {
          oldPosition1 = newPosition1;
          Serial.println(newPosition2);
        }
      }
      else
        {
          flag1=1;
          analogWrite(RIGHT_MOTOR_2,0); 
          analogWrite(RIGHT_MOTOR_1,0);
       }
    }
     //r+=1;     
}

void turn_left()
{
    // turn left and go one block ahead
    // need to do some axis checking with maze and edit
    
    //   Distance between centres of the 2 wheels~~101mm
    //   Quarter circle traversed by each wheel=pi*50.5/2=79.32mm
    
    int flag1=0;
    int flag2=0;
    long distance=360+abs(myEnc1.read());    //Though actual value is 345, I had to compensate for slipping
    long oldPosition1  = -999;
    long oldPosition2  = -999;
    long newPosition1=0;
    long newPosition2=0;
    
        if (newPosition1 != oldPosition1) 
        {
          oldPosition1 = newPosition1;
          Serial.println(newPosition1);
        }
        if (newPosition2 != oldPosition2) 
        {
          oldPosition2 = newPosition2;
          Serial.println(newPosition2);
        }
    while(flag1+flag2<2)
    {
      if(flag1!=1)
      {
        analogWrite(5,0); 
        analogWrite(RIGHT_MOTOR_1,255);
      }
      if(flag2!=1)
      { 
        analogWrite(LEFT_MOTOR_2,255); 
        analogWrite(LEFT_MOTOR_1,0);
      }
      if(newPosition2<distance)
      {
        newPosition2 = abs(myEnc1.read());
        if (newPosition2 != oldPosition2) 
        {
          oldPosition2 = newPosition2;
          Serial.println(newPosition2);
        }
      }
      else
        {
          flag2=1;
          analogWrite(LEFT_MOTOR_2,0); 
          analogWrite(LEFT_MOTOR_1,0);
        }
      if(newPosition1<distance)
      {
        newPosition1 = abs(myEnc1.read());
        if (newPosition1 != oldPosition1) 
        {
          oldPosition1 = newPosition1;
          Serial.println(newPosition2);
        }
      }
      else
        {
          flag1=1;
          analogWrite(RIGHT_MOTOR_2,0); 
          analogWrite(RIGHT_MOTOR_1,0);
       }
    }
     //l+=1;     
}
 
void drive_straight( float distance_right, float distance_left) //To be included in Forward
{
  static int previous_error = 0;
  static int Kp = 25, Ki = 1, Kd = 4;      // constants for scaling P I D effects (will need adjusting)
  static int error, P, I = 0,  D;          // error variables
  int total;

  error = distance_left - distance_right;
  if((abs(error)<error_threshold)&&abs(error)>0.3)
    {
      P = error * Kp;

      //I = (I + error)*Ki;

      D = (error - previous_error) * Kd;                   // may take out
      previous_error = error;

      total = (P+I+D);

      {
        L_enable_val -= (total);
        L_enable_val = constrain(L_enable_val, 120, 255);   // may need to adjust

        R_enable_val += (total);
        R_enable_val = constrain(R_enable_val, 120, 255);

        analogWrite(LEFT_MOTOR_ENABLE, L_enable_val);      // enable pins and values
                                                           // must be global
        analogWrite(RIGHT_MOTOR_ENABLE, R_enable_val);    
    }
  }
  //Serial.print("R:");
  //Serial.print(R_enable_val);
  //Serial.print(" L:");
  //Serial.println(L_enable_val);
  L_enable_val=255;
  R_enable_val=255;   
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
