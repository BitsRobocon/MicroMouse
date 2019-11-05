/*
 * Encoder.ino
 * Last Update:5/11/19 
 */



#define pi 3.141593
#define re1 2                          //Encoder Pins
//#define re2 3
#define le1 18
//#define le2 19
int inr();                             //Function to determine length of maze traversed by right motor 
int inl();                             //Function to determine length of maze traversed by left motor 
volatile float dist=0;                          
volatile int a=0;                      //Right Encoder readings 
volatile int b=0;                      //Left Encoder readings 
void setup() {
pinMode(re1,INPUT_PULLUP);             //Interrupt Pins Setup
//pinMode(re2,INPUT_PULLUP);  
pinMode(le1,INPUT_PULLUP);  
//pinMode(le2,INPUT_PULLUP);  

Serial.begin(9600);
attachInterrupt(digitalPinToInterrupt(re1),inr,RISING );         //Enters ISR here
//attachInterrupt(digitalPinToInterrupt(re2),inr,RISING );
attachInterrupt(digitalPinToInterrupt(le1),inr,RISING );
//attachInterrupt(digitalPinToInterrupt(le2),inr,RISING );
}

/*Some data obout the encoder motor we are using 
 * No. of Readings per revolution if run at full speed~~205
 * Wheel diameter~~30 mm
 * Distance Traversed=(a/205)*pi*30
 */
 
int inr()
{
  if (digitalRead(re1) == HIGH)                   //ISR for right motor
  a++;                                            //Only 1 interrupt used as 2nd interrupt was adding to redundacy
  if (digitalRead(re1) == LOW)
  a--;                                            //a keeps track of the rotary motion of the right encoder
  dist=(a/205)*pi*30                              //distance travelled by right motor in mm
  Serial.println(dist);
  return dist;
}

int inl()
{
  if (digitalRead(le1) == HIGH)
  b++;
  if (digitalRead(le1) == LOW)
  b--;
  dist=(b/205)*pi*30
  Serial.println(dist);
  return dist;
}
