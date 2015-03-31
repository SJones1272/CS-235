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
 
 void update(int w, int h){
   switch(dir){
    case 0 : if(yLoc > 0 - (iconHeight()/2)){yLoc = yLoc - speed;}else{yLoc += (h+speed);} break;
    case 1 : if(xLoc < w - (iconWidth()/2)){xLoc = xLoc + speed;}else{xLoc -= (w+speed);} break;
    case 2 : if(xLoc > 0 - (iconWidth()/2)){xLoc = xLoc - speed;}else{xLoc += (w-speed);} break;
    case 3 : if(yLoc < h - (iconHeight()/2)){yLoc = yLoc + speed;}else{yLoc -= (h+speed);} break; 
   }
 }
 
 int getX(){
  return xLoc; 
 }
 
 int getY(){
  return yLoc; 
 }
 
 int iconWidth()
 {
  return icon.width; 
 }
 
 int iconHeight()
 {
  return icon.height; 
 }
 
 void setDirection(int nd){
  dir = nd; 
 }
  
}
