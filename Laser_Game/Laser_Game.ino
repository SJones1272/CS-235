byte laserByte,photoByte;

int laserLatchPin, laserDataPin, laserClockPin;
int photoLatchPin, photoDataPin, photoClockPin;

void setup()
{
  laserLatchPin = 3;
  laserDataPin = 4;
  laserClockPin = 5;
  
  photoLatchPin = 8;
  photoDataPin = 9;
  photoClockPin = 10;
  laserByte = 0x00;
  photoByte = 0xFF;
  
  Serial.begin(9600);
  pinMode(laserLatchPin,OUTPUT);
  pinMode(laserDataPin,OUTPUT);
  pinMode(laserClockPin,OUTPUT);
  pinMode(photoLatchPin,OUTPUT);
  pinMode(photoClockPin,OUTPUT);
  pinMode(photoDataPin,INPUT);
}

void loop()
{
  checkLasers(); //read in the new values for the resistors
  writeLasers(); //write out the laser states (if changed)
  
  for(int i = 0; i < 8; i++)
  if(nthBit(photoByte,i)==1)
  Serial.println(i);
}

int nthBit(byte b, int n)
{
 return b & (1 << n);
}

byte setNthBit(byte b, int n)
{
 return b | (1 << n); 
}

void setLaser(int i)
{
  laserByte = setNthBit(laserByte,i);
}

void checkLasers()
{
 digitalWrite(photoLatchPin,HIGH);
 delayMicroseconds(20);
 digitalWrite(photoLatchPin,LOW); 
 photoByte = shiftIn(photoDataPin,photoClockPin);
}

void writeLasers()
{
 digitalWrite(laserLatchPin,LOW);
 shiftOut(laserDataPin,laserClockPin,laserByte);
 digitalWrite(laserLatchPin,HIGH); 
}

byte shiftIn(int myDataPin, int myClockPin)
{
 int i;
  int temp = 0;
  int pinState;
  byte myDataIn = 0;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, INPUT);
//we will be holding the clock pin high 8 times (0,..,7) at the
//end of each time through the for loop

//at the begining of each loop when we set the clock low, it will
//be doing the necessary low to high drop to cause the shift
//register's DataPin to change state based on the value
//of the next bit in its serial information flow.
//The register transmits the information about the pins from pin 7 to pin 0
//so that is why our function counts down
  for (i=7; i>=0; i--)
  {
    digitalWrite(myClockPin, 0);
    delayMicroseconds(0.2);
    temp = digitalRead(myDataPin);
    if (temp) {
      pinState = 1;
      //set the bit to 0 no matter what
      myDataIn = myDataIn | (1 << i);
    }
    else {
      //turn it off -- only necessary for debuging
     //print statement since myDataIn starts as 0
      pinState = 0;
    }

    //Debuging print statements
    //Serial.print(pinState);
    //Serial.print("     ");
    //Serial.println (dataIn, BIN);

    digitalWrite(myClockPin, 1);

  }
  //debuging print statements whitespace
  //Serial.println();
  //Serial.println(myDataIn, BIN);
  return myDataIn;
 
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

