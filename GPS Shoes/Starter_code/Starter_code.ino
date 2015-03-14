//Neo-array library

float dest;
int neoPinR; //The control pin for the right foot's neopixels
int neoPinL; //control pin for the left neopixels

//h is the current heading
void showDirection(int h)
{
  showArrow(getDirection(h), /*COLOR*/);
}
//h is current heading
int getDirection(int h)
{
  int temp = h - headingToDest();
  if(temp <= 0)
  {
   return Math.abs(temp); 
  }else
  return Math.abs(temp - 8);
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
