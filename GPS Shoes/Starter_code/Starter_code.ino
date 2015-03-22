#include <Bounce2.h>
#include <TinyGPS++.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      25

#define NEOPIN 6 // The control pin for the neopixels
#define selectPin 9
#define resetPin 7
// Choose two Arduino pins to use for software serial
// The GPS Shield uses D2 and D3 by default when in DLINE mode
int RXPin = 2;
int TXPin = 3;

const double EIFFEL_TOWER_LAT = 48.85826;
const double EIFFEL_TOWER_LNG = 2.294516;

const int TREASURE = 1;
const int EXER = 2;
const int OPTIONS = 3;

//Krannert 34.289125, -85.187522
//Library 34.285816, -85.189213
//Ford Reflecting Pool 34.295339, -85.187894
//Jones 34.285776, -85.186251
double hardLat[4] = {34.289125, 34.285816, 34.295339, 34.285776};
double hardLng[4] = {-85.187522, -85.189213, -85.187894, -85.186251}; 
// The Skytaq EM-506 GPS module included in the GPS Shield Kit
// uses 4800 baud by default
int GPSBaud = 4800;

// Create a TinyGPS++ object called "gps"
TinyGPSPlus gps;

SoftwareSerial gpsSerial(RXPin, TXPin);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

double destLat;
double destLng;
int brightness = 150;
int curMenu = 0; // the current menu value
int potTwist; //the twist of the potentiometer for selecting things.

Bounce selectButton;
Bounce resetButton;

/* 0 is top menu, 
1 is Treasure Hunt, 
2 is Exercise tracker,
3 is Options, and -1 means the game is running.
*/
void setup()
{
  curMenu = 1;
  //potentiometer pin
  pinMode(A0,INPUT);
  selectButton = Bounce();
  //?????
  pinMode(5,INPUT);
  selectButton.attach(selectPin);
  selectButton.interval(10);
  
  resetButton = Bounce();
  resetButton.attach(resetPin);
  resetButton.interval(10);
  pinMode(6,INPUT);
  pinMode(10,OUTPUT);
  
   // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);
  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
  
  destLat = 48.85826;
  destLng = 2.294516;
  
  //testing stuff
  pixels.begin();
  for(int i = 0; i < 10;i++)
  {
   showArrow(i,pixels.Color(0,0,150));
  pixels.show();
  delay(500);
 pixels.clear(); 
  }
 
  pixels.clear(); 
}

void loop()
{
  while (gpsSerial.available() > 0)
  //Serial.print(gps.encode(gpsSerial.read()));
    if (gps.encode(gpsSerial.read())){
      displayInfo();
    }
    
    if(gps.satellites.value() == 0)
      showX(pixels.Color(brightness,0,0));
    else
    {
      if(curMenu > 0)
      checkMenu();
      else{
      updateBrightness();
      run();
      }
        
  }
  
  //Serial.println(gps.satellites.value());
  
  //pixels.clear();
  /*
  while(curMenu == 0){
   checkMenu();
  }
  */
  
  /*
  while(gps.satellites.value() <= 0){
    //Serial.println(gps.satellites.value());
    pixels.clear();
    showX(pixels.Color(150,0,0));
    pixels.show();
  }
  
  if(curMenu == 1){
    startTreasure();
    
    pixels.clear();
   for(int i = 0; i<25; i++){
    pixels.setPixelColor(i, pixels.Color(150, 0, 0)); 
   }
   pixels.show();
   
   
  }
  else if(curMenu == 2){
    pixels.clear();
   for(int i = 0; i<25; i++){
    pixels.setPixelColor(i, pixels.Color(0, 150, 0)); 
   }
   pixels.show();
  }
  //logic for running the game
  else if(curMenu == -1){
     if(atDest()){
      //show some won symbol/time
      //endMode(); 
     }
     else{
      run();
      
     }
  }
  else{
   //throw error 
  }
  */
    // This sketch displays information every time a new sentence is correctly encoded.
   
  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected"));
    while(true);
  } 
}

boolean pressedReset()
{
 return resetButton.fell(); 
}

boolean pressedSelect()
{
 return selectButton.fell(); 
}



//needs to be updated
void run()
{
 showDirection();
 pixels.show(); 
}

//updates buttons and potentiometer
void updateValues()
{
  potTwist = analogRead(A0);
  selectButton.update();
  resetButton.update();
}

//needs to be reworked
void checkMenu()
{
  updateValues();
   int temp = round((potTwist+1)/(1024/4)); 
  // Serial.println(temp);
  // Serial.println(curMenu);
   if(pressedSelect()){
     selectMenu(temp+curMenu);
   }else if(pressedReset())
    reset(); 
}

void selectMenu(int selection)
{
 // curMenu = curMenu + (4 * selection) + 1;
  
 switch (selection){
  case 0 : curMenu = TREASURE; break;// sets the current menu to the value of treasure hunt 
  case 1 : curMenu = EXER; break;
  case 2 : curMenu = OPTIONS; break;
  case 3 : curMenu = 4; break;
  case 4 : startTreasure(); break;
  default : Serial.println("Error"); break;
 }
}
boolean atDest()
{
 return ((abs(gps.location.lat() - destLat) < 0.001) && (abs(gps.location.lng() - destLng) < 0.001));
}

//treasure hunt game
void startTreasure()
{
  //look into better way to get time
 //u32_int startTime = gps.time.value();
 curMenu = -1;
 int r = random(0,4);
 destLat = hardLat[r];
 destLng = hardLng[r];
 Serial.println("Dest lat: "); Serial.print(destLat);
 Serial.println("Dest lng: "); Serial.print(destLng);
}

void reset()
{
 curMenu = 0; //sets the menu back to the top level menu 
}

//changes brightness based on time
void updateBrightness()
{
  int time = gps.time.hour();
 if(time >= 18 || time <= 6)
  brightness = 200;
   else
  brightness = 100; 
}

//h is the current heading
void showDirection()
{
  showArrow(getDirection(), pixels.Color(0,brightness,0));
}

// 0 = forward
// 1 = forward right
// 2 = right
// 3 = down right
// 4 = down
// 5 = down left
// 6 = left
// 7 = up left
void showArrow(int dir, uint32_t c)
{
  //Serial.println(dir);
  switch (dir) {
  case 0 : up(c); break;
  case 1 : upRight(c); break;
  case 2 : right(c); break;
  case 3 : downRight(c); break;
  case 4 : down(c); break;
  case 5 : downLeft(c); break;
  case 6 : left(c); break;
  case 7 : upLeft(c); break;
  case 8 : up(c); break;
  default : showX(c); break;
  }
}
/*
void endMode()
{
 switch (curMode){
  case TREASUREMODE : endTreasure(); break;
  case EXERCISEMODE : endExercise(); break;
 }
}
*/
//h is current heading
int getDirection()
{
  int h = getHeading();
  if(h == 8)
    h = 0;
  
  int temp = h - headingToDest();
  if(temp <= 0)
  {
   return abs(temp); 
  }else
  return abs(temp - 8);
}

/**
returns a numerical destination
0 or 8 -> N
1 -> NE
2 -> E
3 -> SE
4 -> S
5 -> SW
6 -> W
7 -> NW
*/
int getHeading()
{
 double temp = gps.course.deg();
  return round(temp/ 45);

}

int headingToDest()
{
 double heading = gps.courseTo(
    gps.location.lat(),
    gps.location.lng(),
    destLat,
    destLng);
   return round(heading/45); 
}














  
void up(uint32_t color){
 pixels.setPixelColor(2,color); 
 pixels.setPixelColor(7,color);
 pixels.setPixelColor(12,color);
 pixels.setPixelColor(17,color); 
 pixels.setPixelColor(22,color); 
 
 pixels.setPixelColor(16,color);
 pixels.setPixelColor(18,color);
 pixels.setPixelColor(10,color);
 pixels.setPixelColor(14,color);
}

void down(uint32_t color)
{
 pixels.setPixelColor(2,color); 
 pixels.setPixelColor(7,color);
 pixels.setPixelColor(12,color);
 pixels.setPixelColor(17,color); 
 pixels.setPixelColor(22,color); 

 pixels.setPixelColor(6,color);
 pixels.setPixelColor(8,color);
 pixels.setPixelColor(10,color);
 pixels.setPixelColor(14,color);
}

void left(uint32_t color){
  pixels.setPixelColor(10,color);
  pixels.setPixelColor(11,color);
  pixels.setPixelColor(12,color);
  pixels.setPixelColor(13,color);
  pixels.setPixelColor(14,color);
  
  pixels.setPixelColor(8,color);
  pixels.setPixelColor(18,color);
  pixels.setPixelColor(2,color);
  pixels.setPixelColor(22,color);    
}

void right(uint32_t color)
{
  pixels.setPixelColor(10,color);
  pixels.setPixelColor(11,color);
  pixels.setPixelColor(12,color);
  pixels.setPixelColor(13,color);
  pixels.setPixelColor(14,color);
  
  pixels.setPixelColor(6,color);
  pixels.setPixelColor(16,color);
  pixels.setPixelColor(2,color);
  pixels.setPixelColor(22,color);
}

void upRight(uint32_t color)
{
 pixels.setPixelColor(0,color);
 pixels.setPixelColor(8,color);
 pixels.setPixelColor(12,color);
 pixels.setPixelColor(16,color);
 pixels.setPixelColor(24,color);
 
 pixels.setPixelColor(23,color);
 pixels.setPixelColor(15,color); 
 pixels.setPixelColor(22,color);
 pixels.setPixelColor(14,color); 
}

void upLeft(uint32_t color)
{
 pixels.setPixelColor(4,color);
 pixels.setPixelColor(6,color);
 pixels.setPixelColor(12,color);
 pixels.setPixelColor(18,color);
 pixels.setPixelColor(20,color);
 
 pixels.setPixelColor(21,color);
 pixels.setPixelColor(19,color); 
 pixels.setPixelColor(22,color);
 pixels.setPixelColor(10,color); 
}

void downLeft(uint32_t color)
{
 pixels.setPixelColor(0,color);
 pixels.setPixelColor(8,color);
 pixels.setPixelColor(12,color);
 pixels.setPixelColor(16,color);
 pixels.setPixelColor(24,color);
 
 pixels.setPixelColor(9,color);
 pixels.setPixelColor(1,color); 
 pixels.setPixelColor(10,color);
 pixels.setPixelColor(2,color); 
}

void downRight(uint32_t color)
{
 pixels.setPixelColor(4,color);
 pixels.setPixelColor(6,color);
 pixels.setPixelColor(12,color);
 pixels.setPixelColor(18,color);
 pixels.setPixelColor(20,color);
 
 pixels.setPixelColor(3,color);
 pixels.setPixelColor(5,color); 
 pixels.setPixelColor(14,color);
 pixels.setPixelColor(2,color); 
}

void showX(uint32_t color)
{ 
 pixels.setPixelColor(0,color);
 pixels.setPixelColor(8,color);
 pixels.setPixelColor(18,color);
 pixels.setPixelColor(24,color);
 
 pixels.setPixelColor(4,color);
 pixels.setPixelColor(6,color);
 pixels.setPixelColor(12,color);
 pixels.setPixelColor(16,color);
 pixels.setPixelColor(20,color);
}

void showCross(uint32_t color){
  pixels.setPixelColor(10,color);
  pixels.setPixelColor(11,color);
  pixels.setPixelColor(12,color);
  pixels.setPixelColor(13,color);
  pixels.setPixelColor(14,color);
  
 pixels.setPixelColor(2,color); 
 pixels.setPixelColor(7,color);
 pixels.setPixelColor(17,color); 
 pixels.setPixelColor(22,color);
}


void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    
    Serial.print(" Current Heading: ");
    Serial.print(getHeading());
    Serial.print(" Heading to dest: ");
    Serial.print(headingToDest());
    
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
