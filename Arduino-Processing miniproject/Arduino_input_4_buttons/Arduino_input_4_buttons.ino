#include <Bounce2.h>
const int numButtons = 4;
Bounce b[numButtons];
void setup()
{
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
  for(int i =0; i < numButtons; i++)
  b[i].update();
  
  for(int i =0; i < numButtons; i++)
  if(b[i].rose())
  {
   Serial.write(getCode(i)); 
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
