/* More to come... */

int lightState; //current byte value for the 8 leds.
int dataArray[8]; //stores the values for turning on/off specific bits
//NOTE :: might need to change from byte arrays to int arrays 

//for the shift register
int latchPin = 13;
int dataPin = 13;
int clockPin = 13;
int led9Pin = 13;
int count = 1;

boolean tempState = false;
boolean led[8];

void setup(){
  // 0xFF -- 11111111
  dataArray[0] = 0x01;  //00000001
  dataArray[1] = 0x02;  //00000010
  dataArray[2] = 0x04;  //00000100
  dataArray[3] = 0x08;  //00001000
  dataArray[4] = 0x10;  //00010000
  dataArray[5] = 0x20;  //00100000
  dataArray[6] = 0x40;  //01000000
  dataArray[7] = 0x80;  //10000000
  
  lightState = false;
  for(int i = 2; i < 10; i++)
  {
   pinMode(i,OUTPUT); 
  }
  
  for(int j = 0; j< 9; j++){
    led[j] = false;
  }
  Serial.begin(9600);
  digitalWrite(2,1);
}


void loop(){

  
   delay(250);
  for(int i = 1; i < 9;i++){
    setLED(i,true);
  }
  setLED(count,false);    

  for(int i = 2; i < 10; i++)
  {
   digitalWrite(i,led[i-2]); 
  }
 delay(250);
  setLED(count,false);

  for(int i = 2; i < 10; i++)
  {
   digitalWrite(i,led[i-2]); 
  }

  count++;
  if(count > 8)
  count = 1;


}

void setLED(int ledToSet, boolean state)
  {
    digitalWrite(latchPin, 0);
    if(ledToSet == 9)
    {
      digitalWrite(led9Pin,state);
    }else if(state)
    {
      lightState = lightState | dataArray[ledToSet-1];
    }
    else {
      lightState = lightState & (~ dataArray[ledToSet-1]); //bitwise not is ~
    }
    
    int Astate = ithBit(ledToSet-1,lightState,state);
    Serial.print("LightState: ");
    Serial.print(lightState);
    Serial.print(" BitState: ");
    Serial.println(Astate);
    led[ledToSet-1] = Astate;
    //shiftOut(dataPin, clockPin, lightState);
    digitalWrite(latchPin,1);
  }
  
  boolean ithBit(int i,int b,boolean state)
  {
    if(state){
    return (0 < (dataArray[i] | b));
    }else{
    return (0 < (~ dataArray[i] & ~b));
  }
  /*
    double p = pow(2,i);
    Serial.print("Temp : ");
    Serial.println(p);
    int temp;
    if(b!=0)
    temp = (b % round(p));
    else
    return false;
    
    if(temp == 0)
   return true;
   else
   return false;
*/  
}
  
