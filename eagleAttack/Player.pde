class Player{
  
  PImage icon;
  int xLoc, yLoc, dir;
  int speed = 3;
  /*
  0 = up
  1 = right
  2 = left
  3 = down
  
  */
  
  
 Player(int speed, PImage icon, int xLoc, int yLoc, int dir){
   this.icon = icon;
   this.xLoc = xLoc;
   this.yLoc = yLoc;
   this.speed = speed;
   this.dir = dir;
 }
 
 void draw(){
  image(icon, xLoc, yLoc);
  
 }
 
 void update(){
   switch(dir){
    case 0 : yLoc = yLoc - speed; break;
    case 1 : xLoc = xLoc + speed; break;
    case 2 : xLoc = xLoc - speed; break;
    case 3 : yLoc = yLoc + speed; break; 
   }
 }
 
 int getX(){
  return xLoc; 
 }
 
 int getY(){
  return yLoc; 
 }
 
 void setDirection(int nd){
  dir = nd; 
 }
  
}
