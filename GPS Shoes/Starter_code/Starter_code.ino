#include <Adafruit_NeoPixel.h>

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      25

#define NEOPIN 6 //The control pin for the neopixels


// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIN, NEO_GRB + NEO_KHZ800);

float dest;

void setup()
{
  
}

void loop()
{
  
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
0 -> N
1 -> NE
2 -> E
3 -> SE
4 -> S
5 -> SW
6 -> W
7 -> NW
*/
int headingToDest()
{
  return 0;
}
