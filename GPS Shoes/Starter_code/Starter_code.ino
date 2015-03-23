#include <Bounce2.h>
#include <TinyGPS++.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      25

#define NEOPIN 6 // The control pin for the neopixels
#define selectPin 9
#define resetPin 13
// Choose two Arduino pins to use for software serial
// The GPS Shield uses D2 and D3 by default when in DLINE mode
int RXPin = 2;
int TXPin = 3;

const double EIFFEL_TOWER_LAT = 48.85826;
const double EIFFEL_TOWER_LNG = 2.294516;

const int TREASURE = 1;
const int EXER = 2;

//Krannert 34.289125, -85.187522
//Library 34.285816, -85.189213
//Ford Reflecting Pool 34.295339, -85.187894
//Jones 34.285776, -85.186251
float hardLat[4] = {34.289125, 34.285816, 34.295339, 34.285776};
float hardLng[4] = {-85.187522, -85.189213, -85.187894, -85.186251}; 
// The Skytaq EM-506 GPS module included in the GPS Shield Kit
// uses 4800 baud by default
int GPSBaud = 4800;

// Create a TinyGPS++ object called "gps"
TinyGPSPlus gps;

SoftwareSerial gpsSerial(RXPin, TXPin);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

float destLat;
float destLng;
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
  curMenu = 0;
  //potentiometer pin
  pinMode(A0,INPUT);
  selectButton = Bounce();
  //?????
  //pinMode(5,INPUT);
  selectButton.attach(selectPin);
  selectButton.interval(10);
  
  
  resetButton = Bounce();
  resetButton.attach(resetPin);
  resetButton.interval(5);
 // pinMode(6,INPUT);
  //pinMode(10,OUTPUT);
  
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
   showArrow(i,pixels.Color(0,150,0));
  pixels.show();
  delay(500);
 pixels.clear(); 
  }

  pixels.clear(); 
  
for(int i = 0; i < 1000; i++){
  while (gpsSerial.available() > 0)
  Serial.print(gps.encode(gpsSerial.read()));
  pixels.clear();
  showX(pixels.Color(0,brightness, 0));
  pixels.show();
    if (gps.encode(gpsSerial.read())){
      displayInfo();
    }
    delay(1);
  }

}

void loop()
{ 
  
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read())){
      displayInfo();
      pixels.clear();
   
      updateValues();
      //Serial.println("Reset: " ); Serial.print(pressedReset());
      if(pressedReset()){
       reset(); 
      }
      
      //treasure hunt
      if(curMenu == -1)
        run();      
      //party mode
      else if(curMenu == -2)
        partyMode();
      //signal mode
      else if(curMenu == -3)
       signalMode(); 
    }
  
  
  while(curMenu == 0){
   checkMenu();
  }
  
  switch(curMenu){
    case 1 : startTreasure(); break;
    case 2 : startExercise(); break;
    case 3 : startParty(); break;
    case 4 : startSignals(); break;
    //no case 5 yet
  }
  
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected"));
    while(true);
  } 
}

void startParty(){
  curMenu = -2;
  int x = 0;
    while(x < 3){
  pixels.clear();
  for(int i = 0; i<25; i++){
   pixels.setPixelColor(i, pixels.Color(0, 255, 0));
  }
  pixels.show();
  x++;
  delay(300);
  }
  pixels.clear();
  
}

void partyMode(){
  pixels.clear();
  for(int i = 0; i<25; i++){
   pixels.setPixelColor(i, pixels.Color(random(0,255), random(0,255), random(0,255))); 
  }
  pixels.show();
}

void startSignals(){
  curMenu = -3;
 
}

void signalMode(){
  pixels.clear();
  for(int i = 0; i<25; i++){
   pixels.setPixelColor(i, pixels.Color(255, 0, 0)); 
  }
  pixels.show();
  delay(1500);
  pixels.clear();
  for(int x = 0; x<25; x++){
   pixels.setPixelColor(x, pixels.Color(255, 102, 0)); 
  }
  pixels.show();
  delay(1500);
  
}

void startExercise(){
  startTreasure();
}

boolean pressedReset()
{
 return resetButton.rose(); 
}

boolean pressedSelect()
{
 return selectButton.fell(); 
}



//needs to be updated
void run()
{
  if(atDest()){
   curMenu = 0;
   int x = 0;
   while(x < 4){
   for(int i = 0; i<25; i++){
    pixels.setPixelColor(i, pixels.Color(0,0,brightness));
   } 
   x++;
   delay(1000);
   }
   
  }
 pixels.clear();
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
   int temp = ceil((potTwist+1)/(1024/6)); 
   //Serial.println(temp);

  pixels.clear();
  switch (temp) {
  case 0 :  for(int i = 0; i< 25; i++){
     pixels.setPixelColor(i, pixels.Color(0,255,min(255, (i*5)+5))); 
    } break;
  case 1 : showT(); break;
  case 2 : showE(); break;
  case 3 : showP(); break;
  case 4 : showW(); break;
  case 5 : showT(); break;
  case 6 : showT(); break;

  }
  pixels.show();
  /*
  else if(temp == 1){
   pixels.clear();
   showT();
   pixels.show(); 
  }
  else if(temp == 2){
   pixels.clear();
   showE();
   pixels.show(); 
  }
  else if(temp == 3){
   pixels.clear();
   showP();
   pixels.show(); 
  }
  else if(temp == 6){
   pixels.clear();
   showO();
   pixels.show(); 
  }
  */
   if(pressedSelect()){
     selectMenu(temp+curMenu);
   }else if(pressedReset())
    reset(); 
}

void selectMenu(int selection)
{
 // curMenu = curMenu + (4 * selection) + 1;
  
 switch (selection){
  case 0 : curMenu = 0; break;// sets the current menu to the value of treasure hunt 
  case 1 : curMenu = 1; break;
  case 2 : curMenu = 2; break;
  case 3 : curMenu = 3; break;
  case 4 : curMenu = 4; break;
  case 5 : curMenu = 1; break;
  case 6 : curMenu = 6; break;
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
 destLat = 34.295339;
 destLng = -85.187894;
 Serial.print("Dest lat: "); Serial.println(destLat,6);
 Serial.print("Dest lng: "); Serial.println(destLng,6);
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

void endMode()
{
  /*
 switch (curMode){
  case 1 : endTreasure(); break;
  case 2 : endExercise(); break;
 }
 */
}

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

void showT(){
 pixels.setPixelColor(24,pixels.Color(255,102,0));
 pixels.setPixelColor(23,pixels.Color(255,102,0));
 pixels.setPixelColor(22,pixels.Color(255,102,0));
 pixels.setPixelColor(21,pixels.Color(255,102,0));
 pixels.setPixelColor(20,pixels.Color(255,102,0));
 
 pixels.setPixelColor(17,pixels.Color(255,102,0));
 pixels.setPixelColor(12,pixels.Color(255,102,0)); 
 pixels.setPixelColor(7,pixels.Color(255,102,0));
 pixels.setPixelColor(2,pixels.Color(255,102,0)); 
  
}

void showE(){
 pixels.setPixelColor(24,pixels.Color(255,119,255));
 pixels.setPixelColor(23,pixels.Color(255,119,255));
 pixels.setPixelColor(22,pixels.Color(255,119,255));
 pixels.setPixelColor(21,pixels.Color(255,119,255));
 pixels.setPixelColor(20,pixels.Color(255,119,255));
 
 pixels.setPixelColor(19,pixels.Color(255,119,255));
 pixels.setPixelColor(10,pixels.Color(255,119,255));
 pixels.setPixelColor(9,pixels.Color(255,119,255));
 pixels.setPixelColor(0,pixels.Color(255,119,255));
 
 pixels.setPixelColor(11,pixels.Color(255,119,255));
 pixels.setPixelColor(12,pixels.Color(255,119,255));
 pixels.setPixelColor(1,pixels.Color(255,119,255));
 pixels.setPixelColor(2,pixels.Color(255,119,255));
 pixels.setPixelColor(3,pixels.Color(255,119,255));
 pixels.setPixelColor(4,pixels.Color(255,119,255));
}

void showW(){
 pixels.setPixelColor(20,pixels.Color(255,0,0));
 pixels.setPixelColor(19,pixels.Color(255,0,0));
 pixels.setPixelColor(10,pixels.Color(255,0,0));
 pixels.setPixelColor(9,pixels.Color(255,0,0));
 
 pixels.setPixelColor(24,pixels.Color(255,0,0));
 pixels.setPixelColor(15,pixels.Color(255,0,0));
 pixels.setPixelColor(14,pixels.Color(255,0,0));
 pixels.setPixelColor(5,pixels.Color(255,0,0));
 
 pixels.setPixelColor(1,pixels.Color(255,0,0));
 pixels.setPixelColor(2,pixels.Color(255,0,0));
 pixels.setPixelColor(3,pixels.Color(255,0,0));
 pixels.setPixelColor(7,pixels.Color(255,0,0));
 pixels.setPixelColor(12,pixels.Color(255,0,0));

}

void showP(){
 pixels.setPixelColor(20,pixels.Color(134,1,175));
 pixels.setPixelColor(19,pixels.Color(134,1,175));
 pixels.setPixelColor(10,pixels.Color(134,1,175));
 pixels.setPixelColor(9,pixels.Color(134,1,175));
 pixels.setPixelColor(0,pixels.Color(134,1,175));
 
 pixels.setPixelColor(23,pixels.Color(134,1,175));
 pixels.setPixelColor(22,pixels.Color(134,1,175));
 pixels.setPixelColor(21,pixels.Color(134,1,175));
 pixels.setPixelColor(16,pixels.Color(134,1,175));
 pixels.setPixelColor(13,pixels.Color(134,1,175));
 pixels.setPixelColor(11,pixels.Color(134,1,175));
 pixels.setPixelColor(12,pixels.Color(134,1,175));

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
