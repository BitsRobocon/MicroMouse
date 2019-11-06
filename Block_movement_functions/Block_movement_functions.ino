int inr();
int inl();
# define rm1 
# define rm2 
# define lm1 
# define lm2 
void setup() {
 pinMode(rm1,OUTPUT);
 pinMode(rm2,OUTPUT);
 pinMode(lm1,OUTPUT);
 pinMode(lm2,OUTPUT);
}

void Fwd() {                                //Maze block of 16 cm
 int a;                                     //Flag to check if Rmotor has moved by 16cm
 int b;                                     //Flag to check if Lmotor has moved by 16cm
 while(a+b<2)
 {
  if(a!=1)
  {
   digitalWrite(rm1,HIGH);                  //Move fwd until 16cm moved 
   digitalWrite(rm2,LOW);
  }
  if(b!=1)
  {
    digitalWrite(lm1,HIGH);                //Move fwd until 16cm moved
    digitalWrite(lm2,LOW);
  }
  if(inr()%160==0)                         //Check
  int a=1;
  if(inl()%160==0)
  int b=1;
 }
}
/*Distance between centres of the 2 wheels~~101mm
 *Quarter circle traversed by each wheel=pi*50.5/2=79.32mm
 */
void Right(){                              //Rotary motion about Perpendicular to centre of line connecting both wheels
 int b;
 while(b<1)
 {
  if(b!=1)
  { digitalWrite(rm1,LOW);
    digitalWrite(rm2,HIGH);
    digitalWrite(lm1,HIGH);
    digitalWrite(lm2,LOW);
  }
  if(inl()%79==0)                         //Move until a quarter circle of 79mm is moved
  int b=1;
 }
}


void LEFT(){
 int a;
 while(a<1)
 {
  if(a!=1)
  { digitalWrite(rm1,HIGH);
    digitalWrite(rm2,LOW);
    digitalWrite(lm1,LOW);
    digitalWrite(lm2,HIGH);
  }
  if(inr()%79==0)
  int a=1;
 }
}
