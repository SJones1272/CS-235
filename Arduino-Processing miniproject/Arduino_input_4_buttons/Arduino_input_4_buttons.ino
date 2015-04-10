#include <Bounce2.h>
const int numButtons = 4;
Bounce b[numButtons];
int potVal = 0;
int timestamp = 0;
void setup()
{
  pinMode(A0,INPUT);
  for(int i =0; i < numButtons; i++)
  {
    b[i] = Bounce();
   b[i].attach(8+i);
  b[i].interval(10); 
  }
  Serial.begin(9600);
}

void loop()
{
  /*
   int valueP = potVal;
   for( int i = 0; i < 60 ; i++)
   {
     int readV = analogRead(A0);
     if(readV==potVal) break;
     else valueP = readV;
     delay(5);
   }
 int temp =  map(valueP,0,1023,100,612);
 if(temp != potVal){
 potVal = temp;
 Serial.write(potVal);
 }
 */
 
  for(int i =0; i < numButtons; i++)
  b[i].update();
  
  for(int i =0; i < numButtons; i++)
  if(b[i].rose())
  {
   Serial.write(i); 
  }
}

int getCode(int i)
{/*
100 = up
101 = right
102 = left
103 = down
*/
 return 100 + i;
}
