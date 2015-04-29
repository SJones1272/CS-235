#include <Adafruit_NeoPixel.h>

# define NUMPIXELS 20
# define neoPin 7
# define BRIGHTNESS 50
# define LASER_THRESHHOLD 180
# define NUMLASERS 8
# define RESETTIME 8000 // 8 seconds
# define MAPPINGDELAY 500
# define OPTIONS 3
int gameMode;
int gameStartTime;
int currentTime;
int partyTime;
int partyStep = 0;
//lasers
int currentPhotoReads[8];
int basePhotoReads[8];
boolean laserStates[8] = {false,false,false,false,false,false,false,false};
int abc = 0; //curent game state variable

/* Gamemode 1 variables */
int startSelectTime = 0;
int dodgeCount = 0;
int selecting = 0;
boolean blinking = false;
int TIMELIMIT = 5000; //5 seconds, 10 seconds is enough for a smooth countdown. 
boolean lost = false;

//time between laser blinks
int delayBlink = 150;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, neoPin, NEO_GRB + NEO_KHZ800);

static const uint8_t analogPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};
static uint8_t mappedPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};
static const uint8_t laserPins[] = {28,29,30,31,32,33,34,35};

void setup()
{
  for(int i = 0; i < NUMLASERS; i++)
  pinMode(laserPins[i],OUTPUT);
  
  /*
  for(int i = 0; i < NUMLASERS; i++)
  writeLaser(i,HIGH);
  delay(60000);
  darkLasers();
  */
  gameStartTime = millis();
  Serial.begin(9600);
  pixels.begin();
  delay(100);
  pixels.clear();
  pixels.show(); 
 
  mapPins();     // associates the lasers with the analog pins they are pointing at.
  initPixels();  // turns the neopixels on and waits for the photoresitors to adjust to them
  delay(5000);
  calibratePhoto(); //sets the base values for the photoresistors according to the light levels
  
  gameMode = 0; //start the menu option
  abc = 0;
  clearLasers(); //ensure any lasers are in the off states in the laserStates array
  pixels.clear();
  pixels.show();
  pushLasers();  //sends out the laserStates values to the pins
}

void loop(){
  if(lost){
    pixels.clear();
    setAllPixels(pixels.Color(BRIGHTNESS,0,0));
    pixels.show();
    delay(RESETTIME);
    dodgeCount = 0;
    selecting = 0;
   lost = false; 
   abc = 0;
   clearLasers();
   pushLasers();
   delay(500);
   startSelectTime = millis();
  }
  if(selecting == 0)
    startSelectTime = millis();
  while(selecting == 0){
    setAllPixels(pixels.Color(BRIGHTNESS,0,BRIGHTNESS));
    for(int i = 0; i < OPTIONS; i++)
    {
     writeLaser(i,HIGH); 
    }
    //if(millis() - startSelectTime <= 50)
    delay(250);
    updatePhoto();
    
    if(selectionBlock(0))
    selecting = 1;
    else if(selectionBlock(1))
    selecting = 2;
    else if(selectionBlock(2))
    {Serial.println(basePhotoReads[2]);
     Serial.println(currentPhotoReads[2]);
    selecting = 3;
    }pixels.show();
  }
  
  if((abc >= 0) && (abc <= 3)){
      if(selecting == 1)
        runDodgeGame();
      else if(selecting == 2)
        runBlockGame();
      //else 
        //runDodgeBlock();
  }
 if(selecting == 3)
   runPartyMode(); 
}

void runDodgeBlock(){selecting = 0;};

/* Initialization Functions ********************************************************************/
void mapPins()
{
  int tempRead[] = {-1,-1,-1,-1,-1,-1,-1,-1};
  int nextRead[] = {-1,-1,-1,-1,-1,-1,-1,-1};
 for(int i = 0; i < NUMLASERS; i++)
  {
    for(int j = 0; j < 8; j++){
    tempRead[j] = analogRead(analogPins[j]); // read the dark
    }
    writeLaser(i,HIGH);
    delay(MAPPINGDELAY);
    for(int j = 0; j < 8; j++)
    {
     nextRead[j] = analogRead(analogPins[j]); 
     Serial.print("Pin A"); Serial.print(j); Serial.print(" start value: "); Serial.print(tempRead[j]); Serial.print(" Laser on value: "); Serial.println(nextRead[j]);
    if((tempRead[j] - nextRead[j]) >= LASER_THRESHHOLD)
    { mappedPins[i] = analogPins[j]; writeLaser(i,LOW); break;}
    }
    writeLaser(i,LOW);
    delay(MAPPINGDELAY);
  }
 /* Debug statement */
    Serial.println("Lasers mapped to the following values: ");
    for(int i = 0; i < NUMLASERS; i++){Serial.print("Laser "); Serial.print(i); Serial.print(" was mapped to resistor "); Serial.println(mappedPins[i]);} 
}

void initPixels()
{
 for(int i = 0; i < NUMPIXELS; i++)
  pixels.setPixelColor(i,pixels.Color(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS)); 
 pixels.show();
}

void calibratePhoto(){ 
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
void clearLasers(){for(int i = 0; i < NUMLASERS; i++){setLaser(i,false);}}
void darkLasers(){for(int i = 0; i < NUMLASERS; i++){writeLaser(i,LOW);}}   //turn off all lasers without changing the laser state variables.
void setLaser(int i, boolean hilo){laserStates[i] = hilo;}                 //update a laser state to be written at the end of loop()
void writeLaser(int i,int hilo){digitalWrite(laserPins[i],hilo);}          //bypass the updating states to write a specific laser now
void stopLights(){pixels.clear();}
void setAllPixels(uint32_t c){for(int i = 0; i < NUMPIXELS; i++){pixels.setPixelColor(i,c);}}

void rainbow(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}



void randomLasers(int incBy)
{
  int diff = 2 + incBy;
  Serial.println(incBy);
  if(diff > 8)
  diff = 8;
  boolean temp[] = {false,false,false,false,false,false,false,false};
  
  int i = 0;
  while(i < diff){
    int r = random(8);
    if(!temp[r]){
    temp[r] = true;
    i++;
    }
  }
  
  for(int i = 0; i < 8; i++){ Serial.print("Laser "); Serial.print(i); Serial.print(" is "); if(temp[i]){Serial.println("ON"); }else{Serial.println("OFF");}
  laserStates[i] = temp[i];}
}

boolean newRound(){return abc == 0;}
boolean timeIsUp(){return currentTime - gameStartTime >= TIMELIMIT;};
boolean laserOn(int i){return (basePhotoReads[i] - currentPhotoReads[i]) >= LASER_THRESHHOLD;}
boolean isBlocked(int n){return laserStates[n] && !laserOn(n);}
boolean selectionBlock(int i){return !laserOn(i);}
boolean notBlocked(int i){return laserStates[i] && laserOn(i);}
boolean anyBlocked() //should be called after updating photoresistor values.
{
 for(int i = 0; i < NUMLASERS; i++){
  if(isBlocked(i)){Serial.print("Laser "); Serial.print(i); Serial.println(" is Blocked");
   return true;}
 }
  return false; 
}
boolean allBlocked(){for(int i = 0; i < NUMLASERS; i++){if(notBlocked(i)){return false;}}return true;}

/* GAME LOGIC *****************************************************************************************/

void runPartyMode()
{ Serial.println("Partying");
  uint16_t i, j;
 
  for(j=0; j<256*5; j++) { //5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
   int curTime = millis();
   if((curTime % 50) == 0)
     writeLaser(partyStep,HIGH);
   else if((curTime % 50) == 49)
   {writeLaser(partyStep,LOW);
      partyStep++;
    if(partyStep >= 8)
  partyStep = 0;
  }}
}

void startTimeLights(boolean dodge){
  gameStartTime = millis();
  pixels.clear();
  if(dodge)
  setAllPixels(pixels.Color(0,BRIGHTNESS,0));
  else
  setAllPixels(pixels.Color(0,0,BRIGHTNESS));
  pixels.show();  
  delay(200);
}

void updateLights(boolean blue){
  int tempTime = currentTime - gameStartTime;
  tempTime = map(tempTime, 0, TIMELIMIT, 0, NUMPIXELS);
  pixels.clear();
  //Serial.println(tempTime);
  
 for(int i = 20; i>=(20-tempTime); i--){
  pixels.setPixelColor(i, pixels.Color(BRIGHTNESS,0,0));
 }
 if(!blue)
 for(int i = 0; i<=(20-tempTime); i++){
  pixels.setPixelColor(i, pixels.Color(0,BRIGHTNESS,0));
 }
 else
 for(int i = 0; i<=(20-tempTime); i++){
  pixels.setPixelColor(i, pixels.Color(0,0,BRIGHTNESS));
 }
 pixels.show();
}

void updateLightsUp(boolean blue){
  int tempTime = currentTime - gameStartTime;
  tempTime = map(tempTime, 0, TIMELIMIT, NUMPIXELS, 0);
  pixels.clear();
  //Serial.println(tempTime);
  
 for(int i = 20; i>=(20-tempTime); i--){
  pixels.setPixelColor(i, pixels.Color(BRIGHTNESS,0,0));
 }
 if(blue)
 for(int i = 0; i<=(20-tempTime); i++){
  pixels.setPixelColor(i, pixels.Color(0,0,BRIGHTNESS));
 }
 else
 for(int i = 0; i<=(20-tempTime); i++){
  pixels.setPixelColor(i, pixels.Color(0,BRIGHTNESS,0));
 }
 pixels.show();
}

void startRound(boolean dodge){
  randomLasers(floor(dodgeCount/2)); 
  pushLasers(); 
  startTimeLights(dodge);}

void blinkLasers()
{
 int tempTime = currentTime - gameStartTime; 
 if((round(tempTime/1000) % 2) == 0) //if the remainder of tempTime/1000 is even, then turn lasers on, else turn them off
   pushLasers();
 else
   darkLasers();
}

void animateGood(boolean dodge)
{
 int tempTime = currentTime - gameStartTime;
 int timeCheck = (round(tempTime/350) % 3);
  if(timeCheck == 1 || timeCheck == 3){
    pixels.clear();
    pixels.show();
  }else{
    pixels.clear();
    if(dodge)
    for(int i = 0; i < NUMPIXELS; i++)
      pixels.setPixelColor(i,pixels.Color(0,BRIGHTNESS,0));
    else
    for(int i = 0; i < NUMPIXELS; i++)
      pixels.setPixelColor(i,pixels.Color(0,0,BRIGHTNESS));
      
  pixels.show();
  }
}
  
void runDodgeGame(){
currentTime = millis();
  if(newRound()){          //if a new round, generate random lasers, set the start time, and go
     startRound(true); abc++; Serial.println("Starting a new game!");
  }else if(abc == 1)      // show the player the lasers for TIMELIMIT secs and then proceed
  {
   updateLights(false);
   blinkLasers();
   if(timeIsUp()){ Serial.println("Start dodging!");
   abc++; gameStartTime = millis(); pushLasers();}
  }else if(abc == 2)
  {// check if lasers are blocked, if not count up, else do nothing
   updatePhoto();
   //debugPhoto();
   if(anyBlocked()){
    lost = true;   //gameOver() will be called the next time loop initiates.
    return;
   }else{
    updateLightsUp(false); //otherwise count back up in green for TIMELIMIT secs
    if(timeIsUp()){ Serial.println("Good job!");
    abc++; gameStartTime = millis(); clearLasers(); pushLasers();}
   }
  }else if(abc == 3){
    lost = false; // make sure the lost variable is false.
    animateGood(true); //flash green pixels
    if(timeIsUp()){abc = 0; dodgeCount++;} //start over with 1 more dodge
  }else{ //shouldn't happen
    Serial.println("INVALID GAME STATE");
    return;
  }  
}

void runBlockGame()
{
  currentTime = millis();
  if(newRound()){          //if a new round, generate random lasers, set the start time, and go
     startRound(false); abc++; Serial.println("Starting a new game!");
  }else if(abc == 1)      // show the player the lasers for TIMELIMIT secs and then proceed
  {
   updateLights(true);
   blinkLasers();
   if(timeIsUp()){ Serial.println("Start blocking!");
   abc++; gameStartTime = millis(); pushLasers();}
  }else if(abc == 2)
  {// check if lasers are blocked, if not count up, else do nothing
   updatePhoto();
   //debugPhoto();
   if(!allBlocked()){
    lost = true;   //gameOver() will be called the next time loop initiates.
    return;
   }else{
    updateLightsUp(true); //otherwise count back up in green for TIMELIMIT secs
    if(timeIsUp()){ Serial.println("Good job!");
    abc++; gameStartTime = millis(); clearLasers(); pushLasers();}
   }
  }else if(abc == 3){
    lost = false; // make sure the lost variable is false.
    animateGood(false); //flash blue pixels
    if(timeIsUp()){abc = 0; dodgeCount++;} //start over with 1 more dodge
  }else{ //shouldn't happen
    Serial.println("INVALID GAME STATE");
    return;
  }  
}

/* DEBUGGING FUNCTIONS ********************************************************************************/
/* can only be used after mapPins is called */
void debugLasers()
{
 //for debug
 for(int i = 0; i < NUMLASERS; i++){
   setLaser(i,HIGH);
 }
 pushLasers();
  delay(5000); 
updatePhoto();
   
 for(int i = 0 ; i < NUMLASERS; i++)
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
