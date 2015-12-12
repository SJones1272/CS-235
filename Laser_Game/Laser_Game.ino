#include <Adafruit_NeoPixel.h>

# define NUMPIXELS 20
# define neoPin 7
# define BRIGHTNESS 50
# define LASER_THRESHHOLD 180
# define NUMLASERS 8
# define RESETTIME 8000 // 8 seconds
# define MAPPINGDELAY 500
# define OPTIONS 4
# define PARTYTIME 30000 //30 seconds of party time
# define LASER_SFX 1

int gameMode, gameStartTime, currentTime;
int partyStep = 0;
//lasers
int currentPhotoReads[8];
int basePhotoReads[8];
boolean laserStates[8] = {
  false,false,false,false,false,false,false,false};
int abc = 0; //curent game state variable

boolean dodgeBlock = false;

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

static const uint8_t analogPins[] = {
  A0, A1, A2, A3, A4, A5, A6, A7};
static uint8_t mappedPins[] = {
  A0, A1, A2, A3, A4, A5, A6, A7};
static const uint8_t laserPins[] = {
  28,29,30,31,32,33,34,35};

void setup()
{
  for(int i = 0; i < NUMLASERS; i++)
    pinMode(laserPins[i],OUTPUT);

  
  for(int i = 0; i < NUMLASERS; i++)
   writeLaser(i,HIGH);
   delay(30000);
   darkLasers();
   
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
    gameOver();
  }
  if(selecting == 0){
    setAllPixels(pixels.Color(BRIGHTNESS,0,BRIGHTNESS));
    pixels.show();
    for(int i = 0; i < OPTIONS; i++){writeLaser(i,HIGH);}
    delay(5000);
    startSelectTime = millis();
  }
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
      selecting = 3;
    else if(selectionBlock(3))
      selecting = 4;
    pixels.show();
  }

  if((abc >= 0) && (abc <= 3)){
    if(selecting == 1)
      runDodgeGame();
    else if(selecting == 2)
      runBlockGame();
    else if(selecting == 4) 
    runDodgeBlock();
  }
  if(selecting == 3)
    runPartyMode(PARTYTIME); 
}

/* Initialization Functions ********************************************************************/
void mapPins()
{
  int tempRead[] = {
    -1,-1,-1,-1,-1,-1,-1,-1  };
  int nextRead[] = {
    -1,-1,-1,-1,-1,-1,-1,-1  };
  for(int i = 0; i < NUMLASERS; i++)
  {
    for(int j = 0; j < 8; j++){
      tempRead[j] = analogRead(analogPins[j]); // read the dark
    }
    writeLaser(i,HIGH);
    playLaser();
    delay(MAPPINGDELAY);
    for(int j = 0; j < 8; j++)
    {
      nextRead[j] = analogRead(analogPins[j]); 
     /* Serial.print("Pin A"); 
      Serial.print(j); 
      Serial.print(" start value: "); 
      Serial.print(tempRead[j]); 
      Serial.print(" Laser on value: "); 
      Serial.println(nextRead[j]);
     */ if((tempRead[j] - nextRead[j]) >= LASER_THRESHHOLD)
      { 
        mappedPins[i] = analogPins[j]; 
        writeLaser(i,LOW); 
        break;
      }
    }
    writeLaser(i,LOW);
    delay(MAPPINGDELAY);
  }
  /* Debug statement */
 /* Serial.println("Lasers mapped to the following values: ");
  for(int i = 0; i < NUMLASERS; i++){
    Serial.print("Laser "); 
    Serial.print(i); 
    Serial.print(" was mapped to resistor "); 
    Serial.println(mappedPins[i]);
  } */
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
      Serial.print("No laser detected at "); 
      Serial.println(currentPhotoReads[i]); 
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
