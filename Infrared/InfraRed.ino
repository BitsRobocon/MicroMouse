/*
 * InfrRed.ino
 * Last Update:5/11/19
 */

int volt;                      //input voltages from ir
float d;                       //distance measured
int uppthr;                    //threshold values blw which side walls to be considered as present
int frthr;                     //threshold values blw which front wall would be considered as present

//Returned by encoders
int dist;

void setup() {
  pinMode(A0,INPUT);           //Three IRs are being used placed at Front, Right, Left
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  Serial.begin(9600);

}


int f()
{volt=analogRead(A0);
 d=12.08 * pow(volt , -1.058)*250;            //Specific for the model of IR being used
 if(d<frthr)
 return 1;
 else return 0;
}
int r()
{volt=analogRead(A0);
 d=12.08 * pow(volt , -1.058)*250;
 if(d<uppthr)
 return 1;
 else
 return 0;
}
int l()
{volt=analogRead(A0);
 d=12.08 * pow(volt , -1.058)*250;
 if(d<uppthr)
 return 1;
 else
 return 0;
} 
