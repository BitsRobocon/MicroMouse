
/*
 * Block_movement_fuctions_2.ino
 * Author : Hardik Jain (nepython)
 * Last update :29/12/19
 */

#define ERROR_THRESHOLD 5  // To be defined error of threshold
#define RIGHT_MOTOR_ENABLE 6 // enable pin for right motor
#define LEFT_MOTOR_ENABLE 7  // enable pin for left MOTOR
#include <SharpIR.h>
#include <Encoder.h>
// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

// MOTOR
    #define RIGHT_MOTOR_1 4  // pin 1 for right motor
    #define RIGHT_MOTOR_2 5  // pin 2 for right motor
    #define LEFT_MOTOR_1 9   // pin 1 for left motor
    #define LEFT_MOTOR_2 17  // pin 2 for left motor

// ENCODER
    #define RIGHT_ENCODER_DISTANCE 19 // pin for checking distance using right encoder
    #define RIGHT_ENCODER_DIRECTION 18 // pin for checking direction of right encoder
    #define LEFT_ENCODER_DISTANCE 21 // pin for checking distance using left encoder
    #define LEFT_ENCODER_DIRECTION 20 // pin for checking direction of left encoder

Encoder ENCODER_RIGHT(RIGHT_ENCODER_DISTANCE, RIGHT_ENCODER_DIRECTION);
Encoder ENCODER_LEFT(LEFT_ENCODER_DISTANCE, LEFT_ENCODER_DIRECTION);
//   avoid using pins with LEDs attached

//Global Flags
int RIGHT_ENCODER_FLAG=0;      //Encoder flags
int LEFT_ENCODER_FLAG=0;

#define PI 3.141593

//int l=0;      //Flags of previous move
//int f=0;
//int r=0;

int LEFT_MOTOR_SPEED=255;   //ENABLE Pins Values
int RIGHT_MOTOR_SPEED=255;

static int previous_error = 0;    //Error used in PID

void go_forward(long);
void go_backward(long);
void turn_right();
void turn_left();
void drive_straight(float, float);  //Right and Left IR values respectively

#define FRONT_IR_PIN A0
#define LEFT_IR_PIN A1
#define RIGHT_IR_PIN A2
#define IR_MODEL 430
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
  distance=((distance*820)/(3*PI));
  long distance_right=abs(ENCODER_RIGHT.read());
  long distance_left=abs(ENCODER_LEFT.read());

  long OldPositionRight=(-999);
  long OldPositionLeft=(-999);
  long NewPositionRight=0;
  long NewPositionLeft=0;
  while((RIGHT_ENCODER_FLAG+LEFT_ENCODER_FLAG)!=2)
  {
    if (Forward_Distance()<6)
    {
      analogWrite(LEFT_MOTOR_1,0);
      analogWrite(LEFT_MOTOR_2,0);
      analogWrite(RIGHT_MOTOR_1,0);
      analogWrite(RIGHT_MOTOR_2,0);
      return;
    }
    drive_straight(Right_Distance(),Left_Distance());
    if(NewPositionRight<distance*0.5+distance_right)
    {
      NewPositionRight = abs(ENCODER_RIGHT.read());
      if (NewPositionRight != OldPositionRight)
      {
        OldPositionRight = NewPositionRight;
      }
      analogWrite(RIGHT_MOTOR_1,222);
      analogWrite(RIGHT_MOTOR_2,0);
    }
    else if(NewPositionRight>(distance*0.5+distance_right)&&NewPositionRight<(distance*0.7+distance_right))
    {
      analogWrite(RIGHT_MOTOR_1,132);
      analogWrite(RIGHT_MOTOR_2,0);
      NewPositionRight = abs(ENCODER_RIGHT.read());
      if (NewPositionRight != OldPositionRight)
      {
        OldPositionRight = NewPositionRight;
      }
    }
    else if((NewPositionRight>(distance*0.7+distance_right)&&NewPositionRight<(distance+distance_right))&&!(b==1&&previous_error<0.2))
    {
      analogWrite(RIGHT_MOTOR_1,88);
      analogWrite(RIGHT_MOTOR_2,0);
      NewPositionRight = abs(ENCODER_RIGHT.read());
      if (NewPositionRight != OldPositionRight)
      {
        OldPositionRight = NewPositionRight;
      }
    }
    else
    {
      analogWrite(RIGHT_MOTOR_1,0);
      analogWrite(RIGHT_MOTOR_2,0);
      RIGHT_ENCODER_FLAG=1;
    }

    if(NewPositionLeft<distance*0.5+distance_left)
    {
      NewPositionLeft = abs(ENCODER_LEFT.read());
      if (NewPositionLeft != OldPositionLeft)
      {
        OldPositionLeft = NewPositionLeft;
      }
      analogWrite(LEFT_MOTOR_1,255);
      analogWrite(LEFT_MOTOR_2,0);
    }
    else if(NewPositionLeft>(distance*0.5+distance_left)&&NewPositionLeft<(distance*0.7+distance_left))
    {
      analogWrite(LEFT_MOTOR_1,150);
      analogWrite(LEFT_MOTOR_2,0);
      NewPositionLeft = abs(ENCODER_LEFT.read());
      if (NewPositionLeft != OldPositionLeft)
      {
        OldPositionLeft = NewPositionLeft;
      }
    }
    else if((NewPositionLeft>(distance*0.7+distance_left)&&NewPositionLeft<(distance+distance_left))&&!(a==1&&previous_error<0.2))
    {
      analogWrite(LEFT_MOTOR_1,98);
      analogWrite(LEFT_MOTOR_2,0);
      NewPositionLeft = abs(ENCODER_LEFT.read());
      if (NewPositionLeft != OldPositionLeft)
      {
        OldPositionLeft = NewPositionLeft;
      }
    }
    else
    {
      analogWrite(LEFT_MOTOR_1,0);
      analogWrite(LEFT_MOTOR_2,0);
      LEFT_ENCODER_FLAG=1;
    }
    if((a+b)==2)
    {
      RIGHT_ENCODER_FLAG=0;
      LEFT_ENCODER_FLAG=0;
      LEFT_MOTOR_SPEED=255;
      RIGHT_MOTOR_SPEED=255;
      return;
    }
  }
 }

void go_backward(long distance)
{
  distance=((distance*820)/(3*PI))*0.9;
  long distance_right=(ENCODER_RIGHT.read());
  long distance_left=(ENCODER_LEFT.read());
  long OldPositionRight=(-999);
  long OldPositionLeft=(-999);
  long NewPositionRight=(ENCODER_RIGHT.read());
  long NewPositionLeft=(ENCODER_LEFT.read());
  while((RIGHT_ENCODER_FLAG!=1)&&(LEFT_ENCODER_FLAG!=1))                //Some shortcoming is not allowing left motor to ro rotate fully
  {
    drive_straight(Left_Distance(),Right_Distance());
    if(NewPositionRight>-distance*0.5+distance_right)
    {
      NewPositionRight = (ENCODER_RIGHT.read());
      if (NewPositionRight != OldPositionRight)
      {
        OldPositionRight = NewPositionRight;
      }
      analogWrite(RIGHT_MOTOR_2,255);
      analogWrite(RIGHT_MOTOR_1,0);
    }
    else if(NewPositionRight<(-distance*0.5+distance_right)&&NewPositionRight>(-distance*0.7+distance_right))
    {
      analogWrite(RIGHT_MOTOR_2,150);
      analogWrite(RIGHT_MOTOR_1,0);
      NewPositionRight = (ENCODER_RIGHT.read());
      if (NewPositionRight != OldPositionRight)
      {
        OldPositionRight = NewPositionRight;
      }
    }
    else if((NewPositionRight<(-distance*0.7+distance_right)&&NewPositionRight>(-distance*0.85+distance_right)))
    {
      analogWrite(RIGHT_MOTOR_2,123);
      analogWrite(RIGHT_MOTOR_1,0);
      NewPositionRight = abs(ENCODER_RIGHT.read());
      if (NewPositionRight != OldPositionRight)
      {
        OldPositionRight = NewPositionRight;
      }
    }
    else
    {
      analogWrite(RIGHT_MOTOR_2,0);
      analogWrite(RIGHT_MOTOR_1,0);
      RIGHT_ENCODER_FLAG=1;
    }

    if(NewPositionLeft>-distance*0.5+distance_left)
    {
      NewPositionLeft = (ENCODER_LEFT.read());
      if (NewPositionLeft != OldPositionLeft)
      {
        OldPositionLeft = NewPositionLeft;
      }
      analogWrite(LEFT_MOTOR_2,200);
      analogWrite(LEFT_MOTOR_1,0);
    }
    else if(NewPositionLeft<(-distance*0.5+distance_left)&&NewPositionLeft>(-distance*0.7+distance_left))
    {
      analogWrite(LEFT_MOTOR_2,137);
      analogWrite(LEFT_MOTOR_1,0);
      NewPositionLeft = (ENCODER_LEFT.read());
      if (NewPositionLeft != OldPositionLeft)
      {
        OldPositionLeft = NewPositionLeft;
      }
    }
    else if((NewPositionLeft<(-distance*0.7+distance_left)&&NewPositionLeft>(-distance+distance_left)))
    {
      analogWrite(LEFT_MOTOR_2,125);
      analogWrite(LEFT_MOTOR_1,0);
      NewPositionLeft = abs(ENCODER_LEFT.read());
      if (NewPositionLeft != OldPositionLeft)
      {
        OldPositionLeft = NewPositionLeft;
      }
    }
    else
    {
      analogWrite(LEFT_MOTOR_1,0);
      analogWrite(LEFT_MOTOR_2,0);
      LEFT_ENCODER_FLAG=1;
    }
    if((RIGHT_ENCODER_FLAG+LEFT_ENCODER_FLAG)==2)
    {
      RIGHT_ENCODER_FLAG=0;
      LEFT_ENCODER_FLAG=0;
      LEFT_MOTOR_SPEED=255;
      RIGHT_MOTOR_SPEED=255;
      return;
    }
  }
 }

void turn_right()
{
    // turn right and go one block ahead
    // need to do some axis checking with maze and edit

    // Distance between centres of the 2 wheels~~101mm
    // Quarter circle traversed by each wheel=pi*50.5/2=79.32mm

    RIGHT_ENCODER_FLAG=0;
    LEFT_ENCODER_FLAG=0;
    long distance=372*1.02+abs(ENCODER_LEFT.read());
    long OldPositionRight  = -999;
    long OldPositionLeft  = -999;
    long NewPositionRight=0;
    long NewPositionLeft=0;

        if (NewPositionRight != OldPositionRight)
        {
          OldPositionRight = NewPositionRight;
        }
        if (NewPositionLeft != OldPositionLeft)
        {
          OldPositionLeft = NewPositionLeft;
        }
    while(RIGHT_ENCODER_FLAG+LEFT_ENCODER_FLAG<2)
    {
      if(RIGHT_ENCODER_FLAG!=1)
      {
        analogWrite(RIGHT_MOTOR_2,255);
        analogWrite(RIGHT_MOTOR_1,0);
      }
      if(LEFT_ENCODER_FLAG!=1)
      {
        analogWrite(LEFT_MOTOR_2,0);
        analogWrite(LEFT_MOTOR_1,255);
      }
      if(NewPositionLeft<distance)
      {
        NewPositionLeft = abs(ENCODER_LEFT.read());
        if (NewPositionLeft != OldPositionLeft)
        {
          OldPositionLeft = NewPositionLeft;
        }
      }
      else
        {
          LEFT_ENCODER_FLAG=1;
          analogWrite(LEFT_MOTOR_2,0);
          analogWrite(LEFT_MOTOR_1,0);
        }
      if(NewPositionRight<distance)
      {
        NewPositionRight = abs(ENCODER_LEFT.read());
        if (NewPositionRight != OldPositionRight)
        {
          OldPositionRight = NewPositionRight;
        }
      }
      else
        {
          RIGHT_ENCODER_FLAG=1;
          analogWrite(RIGHT_MOTOR_2,0);
          analogWrite(RIGHT_MOTOR_1,0);
       }
    }
}

void turn_left()
{
    // turn left and go one block ahead
    // need to do some axis checking with maze and edit

    //   Distance between centres of the 2 wheels~~101mm
    //   Quarter circle traversed by each wheel=pi*50.5/2=79.32mm

    RIGHT_ENCODER_FLAG=0;
    LEFT_ENCODER_FLAG=0;
    long distance=420+abs(ENCODER_RIGHT.read());    //Though actual value is 345, I had to compensate for slipping
    long OldPositionRight  = -999;
    long OldPositionLeft  = -999;
    long NewPositionRight=0;
    long NewPositionLeft=0;

        if (NewPositionRight != OldPositionRight)
        {
          OldPositionRight = NewPositionRight;
        }
        if (NewPositionLeft != OldPositionLeft)
        {
          OldPositionLeft = NewPositionLeft;
        }
    while(RIGHT_ENCODER_FLAG+LEFT_ENCODER_FLAG<2)
    {
      if(RIGHT_ENCODER_FLAG!=1)
      {
        analogWrite(5,0);
        analogWrite(RIGHT_MOTOR_1,255);
      }
      if(flag2!=1)
      {
        analogWrite(LEFT_MOTOR_2,255);
        analogWrite(LEFT_MOTOR_1,0);
      }
      if(NewPositionLeft<distance)
      {
        NewPositionLeft = abs(ENCODER_RIGHT.read());
        if (NewPositionLeft != OldPositionLeft)
        {
          OldPositionLeft = NewPositionLeft;
        }
      }
      else
        {
          LEFT_ENCODER_FLAG=1;
          analogWrite(LEFT_MOTOR_2,0);
          analogWrite(LEFT_MOTOR_1,0);
        }
      if(NewPositionRight<distance)
      {
        NewPositionRight = abs(ENCODER_RIGHT.read());
        if (NewPositionRight != OldPositionRight)
        {
          OldPositionRight = NewPositionRight;
        }
      }
      else
        {
          RIGHT_ENCODER_FLAG=1;
          analogWrite(RIGHT_MOTOR_2,0);
          analogWrite(RIGHT_MOTOR_1,0);
       }
    }
}

void drive_straight( float distance_right, float distance_left) //To be included in Forward
{
  static int Kp = 32, Ki = 1, Kd = 4;      // constants for scaling P I D effects (will need adjusting)
  static int error, P, I = 0,  D;          // error variables
  int total;

  error = distance_left - distance_right;
  if((abs(error)<ERROR_THRESHOLD)&&abs(error)>0.2)
    {
      P = error * Kp;

      //I = (I + error)*Ki;

      D = (error - previous_error) * Kd;                   // may take out
      previous_error = error;

      total = (P+I+D);

      {
        LEFT_MOTOR_SPEED -= (total);
        LEFT_MOTOR_SPEED = constrain(LEFT_MOTOR_SPEED, 120, 255);   // may need to adjust

        RIGHT_MOTOR_SPEED += (total);
        RIGHT_MOTOR_SPEED = constrain(RIGHT_MOTOR_SPEED, 120, 255);

        analogWrite(LEFT_MOTOR_ENABLE, LEFT_MOTOR_SPEED);      // enable pins and values
                                                           // must be global
        analogWrite(RIGHT_MOTOR_ENABLE, RIGHT_MOTOR_SPEED);
    }
  }
  LEFT_MOTOR_SPEED=255;
  RIGHT_MOTOR_SPEED=255;
}



float Forward_Distance()
{
  SharpIR SharpIR(FRONT_IR_PIN, IR_MODEL);
  return SharpIR.distance();
}

float Left_Distance()
{
  SharpIR SharpIR(LEFT_IR_PIN, IR_MODEL);
  return (SharpIR.distance() + 0.45); //accounting for difference in values
}

float Right_Distance()
{
  SharpIR SharpIR(RIGHT_IR_PIN, IR_MODEL);
  return SharpIR.distance();
}
