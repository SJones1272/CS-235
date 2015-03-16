#include <TinyGPS++.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      25

#define NEOPIN 6 //The control pin for the neopixels

// Choose two Arduino pins to use for software serial
// The GPS Shield uses D2 and D3 by default when in DLINE mode
int RXPin = 2;
int TXPin = 3;

// The Skytaq EM-506 GPS module included in the GPS Shield Kit
// uses 4800 baud by default
int GPSBaud = 4800;

// Create a TinyGPS++ object called "gps"
TinyGPSPlus gps;

SoftwareSerial gpsSerial(RXPin, TXPin);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

double destLat;
double destLng;


void setup()
{
   // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println(); 
}

void loop()
{
    // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read())){
      displayInfo();
    }
  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected"));
    while(true);
  }
}

//h is the current heading
void showDirection(int h)
{
  showArrow(getDirection(h), 32);
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
  switch (dir) {
  case 0 : up(c); break;
  case 1 : upRight(c); break;
  case 2 : right(c); break;
  case 3 : downRight(c); break;
  case 4 : down(c); break;
  case 5 : downLeft(c); break;
  case 6 : left(c); break;
  case 7 : upLeft(c); break;
  default : showX(c); break;
  }
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
 pixels.setPixelColor(18,color);
 pixels.setPixelColor(24,color);
 
 pixels.setPixelColor(23,color);
 pixels.setPixelColor(19,color); 
 pixels.setPixelColor(22,color);
 pixels.setPixelColor(14,color); 
}

void upLeft(uint32_t color)
{
 pixels.setPixelColor(4,color);
 pixels.setPixelColor(6,color);
 pixels.setPixelColor(12,color);
 pixels.setPixelColor(16,color);
 pixels.setPixelColor(20,color);
 
 pixels.setPixelColor(21,color);
 pixels.setPixelColor(15,color); 
 pixels.setPixelColor(22,color);
 pixels.setPixelColor(10,color); 
}

void downLeft(uint32_t color)
{
 pixels.setPixelColor(0,color);
 pixels.setPixelColor(8,color);
 pixels.setPixelColor(12,color);
 pixels.setPixelColor(18,color);
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
 pixels.setPixelColor(16,color);
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
  

//h is current heading
int getDirection(int h)
{
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
