#include <Adafruit_NeoPixel.h>

#define numLasers 9
# define NUMPIXELS 20
# define neoPin 7
# define BRIGHTNESS 50
# define LASER_THRESHHOLD 75

int gameMode;
int gameStartTime;
int currentTime;
//lasers
int currentPhotoReads[8];
int basePhotoReads[8];
boolean laserStates[8] = {false,false,false,false,false,false,false,false};
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
static uint8_t mappedPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};
static const uint8_t laserPins[] = {19,20, 21, 22, 23, 24, 25, 26, 27};

void setup()
{
  
  gameStartTime = millis();
  Serial.begin(9600);
  pixels.begin(); 
  
  mapPins();     // associates the lasers with the analog pins they are pointing at.
  initPixels();  // turns the neopixels on and waits for the photoresitors to adjust to them
  delay(2000);
  calibratePhoto(); //sets the base values for the photoresistors according to the light levels
  
  gameMode = 0; //start the menu option
  clearLasers(); //ensure any lasers are in the off states in the laserStates array
  pushLasers();  //sends out the laserStates values to the pins
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
  
  //pushLasers(); //write out the laser states (if changed
  //runMenu();
}



/* Initialization Functions ********************************************************************/
void mapPins()
{
  int tempRead = -1;
 for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++){
    tempRead = analogRead(analogPins[j]);
    writeLaser(i,HIGH);
    delay(5000);
    if(tempRead - analogRead(analogPins[j] >= LASER_THRESHHOLD))
    { mappedPins[i] = analogPins[j]; writeLaser(i,LOW); break;}
    writeLaser(i,LOW);
    }
  }
 /* Debug statement */
    Serial.println("Lasers mapped to the following values: ");
    for(int i = 0; i < 8; i++){Serial.print("Laser "); Serial.print(i); Serial.print(" was mapped to resistor "); Serial.println(mappedPins[i]);} 
}

void initPixels()
{
 for(int i = 0; i < NUMPIXELS; i++)
  pixels.setPixelColor(i,pixels.Color(0,BRIGHTNESS,0); 
 pixels.show();
}

void calibratePhoto(){
  startUpLights();  
 for(int i = 0; i< 8; i++){
   basePhotoReads[i] = analogRead(mappedPins[i]);
 }
 int r = 0;
 while(r < 3){
   for(int z = 0; z<8; z++){
    basePhotoReads[z] = (basePhotoReads[z] + analogRead(mappedPins[z]))/2; 
   }
   r++;
 }
 
 // detectLasers(); // for debug
 }
/* HELPER FUNCTIONS ***********************************************************************************/
void updatePhoto(){for(int i = 0; i< 8; i++)currentPhotoReads[i] = analogRead(mappedPins[i]);}
void pushLasers(){for(int i = 0; i< 8; i++){digitalWrite(laserPins[i], laserStates[i]);}} //send out the laserState values at the end of loop()
void clearLasers(){for(int i = 0; i < numLasers; i++){setLaser(i,false);}} 
void setLaser(int i, boolean hilo){laserStates[i] = hilo;}                 //update a laser state to be written at the end of loop()
void writeLaser(int i,int hilo){digitalWrite(laserPins[i],hilo);}          //bypass the updating states to write a specific laser now
void stopLights(){pixels.clear();}
void randomLasers(int diff)
{
  if( diff < 2)
  diff = 2;
  boolean temp[] = {false,false,false,false,false,false,false,false};
  
  int i = 0;
  while(i < 8 || i < diff )
  {
    int r = random(8);
    if(!temp[r]){
    temp[r] = true;
    i++;
    }
  }
  
  for(int i = 0; i < 8; i++)
  laserStates[i] = temp[i];
}

boolean laserOn(int i){return LASER_THRESHHOLD <= (basePhotoReads[i] - currentPhotoReads[i]);}
boolean isBlocked(int n){return laserStates[n] && laserOn(n);}
boolean anyBlocked()
{
 for(int i = 0; i < 8; i++)
  if(isBlocked(i))
   return true;
  
  return false; 
}

/* GAME LOGIC *****************************************************************************************/

void startTimeLights(){
  gameStartTime = millis();
  pixels.clear();
  for(int i = 0; i< NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(0,BRIGHTNESS,0));
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
  pixels.setPixelColor(i, pixels.Color(BRIGHTNESS,0,0));
 }
 for(int i = 0; i<=(20-tempTime); i++){
  pixels.setPixelColor(i, pixels.Color(0,BRIGHTNESS,0));
 }
 
 if(tempTime > 21){
  abc = 0;
  delay(500); 
 }

 pixels.show();
}

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
    updatePhoto();
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
/* DEBUGGING FUNCTIONS ********************************************************************************/
/* can only be used after mapPins is called */
void debugLasers()
{
 //for debug
 for(int i = 0; i < 8; i++){
   setLaser(i,HIGH);
 }
 pushLasers();
  delay(5000); 
updatePhoto();
   
 for(int i = 0 ; i < 8; i++)
 if(!laserOn(i)){
   Serial.print("No laser detected at "); Serial.println(currentPhotoReads[i]); 
  }
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
/* OLD CODE ********************************************************************************************* */
/*
void runGame(int game){
  if(game == 0)
    runDodgeGame();
  else if(game == 1)
    runBlockGame(); //for adding more gamemodes
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
