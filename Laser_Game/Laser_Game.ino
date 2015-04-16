#define numLasers 8
byte laserByte; photoByte;

int laserLatchPin, laserDataPin, laserClockPin;
//int photoLatchPin, photoDataPin, photoClockPin;
int photoSelectPin; //S_0 pin for the multiplexor
int photoDataPin, photoLatchPin; //Output wire, and OE wire
int gameMode;
int S_0, S_1, S_2;

/* Gamemode 1 variables */
int blinkTimeStamp = 0;
int dodgeCount = 0;
boolean blinking = false;

void setup()
{
  laserLatchPin = 3;
  laserDataPin = 4;
  laserClockPin = 5;
  photoSelectPin = 6; // 7 and 8 are also select pins S_0, S_1 , S_2
  S_0 = photoSelectPin;
  S_1 = S_0 + 1;
  S_2 = S_1 + 1;
  
  photoLatchPin = 9; //OE pin
  photoDataPin = 10; //input read pin
  laserByte = 0x00;
  
  Serial.begin(9600);
  
  for(int i = 3; i < 9;i++)
  pinMode(i,OUTPUT);
  
  pinMode(photoDataPin,INPUT);

  gameMode = 0; //start the menu option
}

void loop()
{
  checkLasers(); //read in the new values for the resistors
  writeLasers(); //write out the laser states (if changed)
  if(gameMode > 0)
  runGame(gameMode);
  else
  //runMenu();
  
  
  for(int i = 0; i < 8; i++)
  if(nthBit(photoByte,i)==1)
  Serial.println(i); // laser blocked at i
}

void runGame(int game){runBaseGame(); //for adding more gamemodes}

void runBaseGame()
{
  int curTime = millis();
   if(blinking && curTime - blinkTimeStamp <= 5000 && (blinkTimeStamp != 0))
   {
     countDown(curTime);
   }else{
     if(anyBlocked())
     {
       //failure animation, then 
       // gameOver(); or lives--;
     }else
     {
       //Good dodge animation, generate new random lasers
       randomLasers(floor(dodgeCount/3));
     }
   }
 }

void randomLasers(int i)
{
 int baseDiff = 2;
 int numRand = 2 + i;
 
 if(numRand > 8)
 numRand = 8;
 
 clearLasers();
 
 int rand[] = {0,0,0,0,0,0,0,0};
 
  for(int j =0; j < numRand; j++)
 {
   int randomN = round(random(0,8));
   boolean same = false;
   for(int k = 0; k < j; k++)
   {
     if(rand[k] == randomN)
     same = true;
   }
   if(same)
   j--;
   else
   rand[j] = randomN;
 }
 
 for(int j = 0; j < numRand;j++)
 {
  setLaser(rand[j],true); 
 }
}

void clearLasers(){for(int i = 0; i < numLasers; i++){setLaser(i,false);}}

int nthBit(byte b, int n)
{
  if((b & (1 << n)) > 0) return 1;
  else return 0;
}

byte setBit(byte b, int n,boolean value)
{
  if(value)
   return b | (1 << n); 
  else
   return b & !(1 << n);
}

void setLaser(int i,boolean on){laserByte = setBit(laserByte,i,on);}
boolean isBlocked(int n){return (nthBit(photoByte,n) == 1);}

boolean anyBlocked()
{
 for(int i = 0; i < numLasers;i++)
  if(isBlocked(i))
    return false;
  return true;
}

void checkLasers()
{
 digitalWrite(photoLatchPin,LOW); //Enables output to the arduin
 /*
 delayMicroseconds(20);
 digitalWrite(photoLatchPin,LOW); 
 photoByte = shiftIn(photoDataPin,photoClockPin);
 */
 photoByte = readRecs();
 digitalWrite(photoLatchPin,HIGH);
}

byte readRecs()
{
   byte b = 0x00;
   for(int i = 0; i < 8; i++)
   setBit(b,i,(readMultiplex(i)==1));

   return b;
}

int readMultiplex(int i)
{
  digitalWrite(S_0,nthBit(i,0));
  digitalWrite(S_1,nthBit(i,1));
  digitalWrite(S_2,nthBit(i,2));
  
  return digitalRead(photoDataPin);
}

void writeLasers()
{
 digitalWrite(laserLatchPin,LOW);
 shiftOut(laserDataPin,laserClockPin,laserByte);
 digitalWrite(laserLatchPin,HIGH); 
}


/*
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
*/
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

