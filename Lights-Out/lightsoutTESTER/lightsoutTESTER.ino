/* More to come... */

int lightState; //current byte value for the 8 leds.
int dataArray[8]; //stores the values for turning on/off specific bits
//NOTE :: might need to change from byte arrays to int arrays 

//for the shift register
int latchPin = 13;
int dataPin = 11;
int clockPin = 12;
int led9Pin = A0;

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
  
  lightState = 0xFF;
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

 digitalWrite(latchPin,0);
 shiftOut(dataPin,clockPin, lightState);
 digitalWrite(latchPin,1);
  



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

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}
  
