#include <Adafruit_NeoPixel.h>

#define numLasers 9
# define NUMPIXELS 20
# define neoPin 6

int gameMode;
int gameStartTime;
int currentTime;
//lasers
int currentPhotoReads[8];
int basePhotoReads[8];
boolean laserStates[8];
int abc = 0;

/* Gamemode 1 variables */
int blinkTimeStamp = 0;
int dodgeCount = 0;
boolean blinking = false;
int TIMELIMIT = 10000; //10 seconds
boolean lost = false;

//time between laser blinks
int delayBlink = 150;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, neoPin, NEO_GRB + NEO_KHZ800);

static const uint8_t analogPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};
static const uint8_t laserPins[] = {20, 21, 22, 23, 24, 25, 26, 27, 28};
void setup()
{
  
  gameStartTime = millis();

   
  
  Serial.begin(9600);
  pixels.begin();
  for(int i = 3; i < 9;i++)
  pinMode(i,OUTPUT);
  
  
  calibratePhoto();
  
  gameMode = 0; //start the menu option
  for(int r = 0; r<20; r++){
   pixels.setPixelColor(r, pixels.Color(255,0,0));
   pixels.show();
  }
  pixels.show();
  
  
  
}

void updatePhoto(){
  for(int i = 0; i< 8; i++){
   currentPhotoReads[i] = analogRead(analogPins[i]);
 }
}

void calibratePhoto(){
 for(int i = 0; i< 8; i++){
   currentPhotoReads[i] = analogRead(analogPins[i]);
 }
 int r = 0;
 while(r < 3){
   for(int z = 0; z<8; z++){
    currentPhotoReads[z] = (currentPhotoReads[z] + analogRead(analogPins[z]))/2; 
   }
   r++;
 }
 
}

void startTimeLights(){
  gameStartTime = millis();
  pixels.clear();
  for(int i = 0; i< NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0,255,0));
    }
  pixels.show();  
  delay(200);
}

void updateLights(){
  currentTime = millis();
  int tempTime = currentTime - gameStartTime;
  
  tempTime = map(tempTime, 0, TIMELIMIT, 0, NUMPIXELS);
  pixels.clear();
  //Serial.println(tempTime);
  
 for(int i = 20; i>=(20-tempTime); i--){
  pixels.setPixelColor(i, pixels.Color(255,0,0));
 }
 for(int i = 0; i<=(20-tempTime); i++){
  pixels.setPixelColor(i, pixels.Color(0,255,0));
 }
 
 if(tempTime > 21){
  abc = 0;
  delay(500); 
 }

 pixels.show();
}

void stopLights(){
 pixels.clear(); 
}
void loop()
{
  if(abc == 0){
   startTimeLights();
    abc++; 
  }
  else{
   updateLights(); 
  }
  
  //updatePhoto();
  //debugPhoto();
  //delay(2000);
  
  //writeLasers(); //write out the laser states (if changed
  //runMenu();
}

void debugPhoto(){
 
  for(int i = 0; i< 8; i++){
   Serial.print("A");
   Serial.print(i);
   Serial.print(": ");
   Serial.print("Base: ");
   Serial.print(basePhotoReads[i]);
   Serial.print("   current: ");
   Serial.println(currentPhotoReads[i]);
   }
  delay(200); 
  Serial.println("===============================");
}

void updateLasers(){
 for(int i = 0; i< 8; i++){
  digitalWrite(laserPins[i], laserStates[i]);
 } 
}

/*
void runGame(int game){
  if(game == 0)
    runDodgeGame();
  else if(game == 1)
    runBlockGame(); //for adding more gamemodes
}

//runs the dodge game
void runDodgeGame(){
  //start the game
  //generate random lasers
  randomLasers(floor(dodgeCount/3));
  
  for(int i = 0; i< 3; i++){
    delay(delayBlink);
  
  }
  startTimeLights();
  updateLights();
  
  while(currentTime - gameStartTime < TIMELIMIT){
    checkLasers();
   if(!anyBlocked()){
    lost = true ;
    break;
   }else{
    updateLights();
   } 
  }
  
  if(!lost){
  dodgeCount++;
  }
  else{
    dodgeCount = 0;
    lost = false;
  } 
}
//runs the block game
void runBlockGame(){
  //start the game
  //generate random lasers
  randomLasers(floor(dodgeCount/3));
  
  for(int i = 0; i< 3; i++){
   
  }
  startTimeLights();
  updateLights();
  
  while(currentTime - gameStartTime < TIMELIMIT){
    checkLasers();
   if(anyNotBlocked()){
    lost = true;
    break;
   }else{
    updateLights();
   } 
  }
  
  if(!lost){
  dodgeCount++;
  }
  else{
    dodgeCount = 0;
    lost = false;
  }
}


void runBaseGame()
{
   int curTime = millis();
   if(blinking && curTime - blinkTimeStamp <= 5000 && (blinkTimeStamp != 0))
   {
     //countDown(curTime);
   }else{
     if(anyBlocked())
     {
       stopLights();
       for(int i = 0; i< 3; i++){
         pixels.clear();
        for(int r = 0; r< NUMPIXELS; r++){
         pixels.setPixelColor(r, pixels.Color(0,0,255));
        }
        pixels.show();
        delay(150);
       }
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
boolean isNotBlocked(int n){return (nthBit(photoByte, n) == 0);}

boolean anyNotBlocked(){
 for(int i = 0; i<numLasers; i++)
 {
   if(isNotBlocked(i))
     return false;
 }
 return true;
 }

boolean anyBlocked()
{
 for(int i = 0; i < numLasers;i++)
  if(isBlocked(i))
    return false;
  return true;
}


void checkLasers()
{
  for(int i = 0; i< 8; i++){
    
  }
  /*
 digitalWrite(photoLatchPin,HIGH); //Enables output to the arduin
 delayMicroseconds(20);
 digitalWrite(photoLatchPin,LOW); 
 photoByte = shiftIn(photoDataPin,photoClockPin);
 
}
*/





