/* More to come... */

byte lightState; //current byte value for the 8 leds.
byte dataArray[8]; //stores the values for turning on/off specific bits


//for the shift register
int latchPin;
int dataPin;
int clockPin;

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
  
}


void loop(){
  
  
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
    
    shiftOut(dataPin, clockPin, lightState);
    digitalWrite(latchPin,1);
  }
