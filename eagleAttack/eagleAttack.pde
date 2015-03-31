import processing.serial.*;

Player p1, p2;
Serial myport;
String port = "COM3"; //"/dev/tty.usbmodem1411";
int controller = -1;
int hitRange = 40;
boolean gameOver;
//PImage sprite_sheet = loadImage("squirrelSprites.png", "png");
PImage bkg;
void setup(){
  bkg = loadImage("http://www.georgia.org/SiteCollectionImages/Industries/Entertainment/Camera%20Ready/Counties/Floyd/Berry%20College.jpg","jpg");
    size(bkg.width, bkg.height);
 PImage temp = loadImage("http://vignette3.wikia.nocookie.net/sims/images/4/41/Squirrel.png/revision/latest?cb=20111022014337", "png");
  temp.resize(60,40);
  PImage eagle = loadImage("eagle.png", "png");
  eagle.resize(150,90);
  p1 = new Player(3,temp, 0,0,3);
  p2 = new Player(6,eagle, width/2, height/2, 0);
  
  myport = new Serial(this, port, 9600); 
  gameOver = false;
  
 
  
}

int distance(int x1, int y1,int x2,int y2){
 return round(sqrt(sq(x1-x2) + sq(y1-y2))); 
}

void draw(){
image(bkg,0,0);
  if(gameOver){
  
  
 }
else{
 pressedKey(); 
  if(myport.available() > 0)
  {
   controller = myport.read(); 
  p1.setDirection(controller);  
  }
  
 p1.update(width,height);
 p2.update(width,height);
 if(distance(p1.getX()+(p1.iconWidth()/2), p1.getY()+(p1.iconHeight()/2), p2.getX()+(p2.iconWidth()/2), p2.getY()+(p2.iconHeight()/2)) <= hitRange)
   gameOver = true;
   
 println(distance(p1.getX()+(p1.iconWidth()/2), p1.getY()+(p1.iconHeight()/2), p2.getX()+(p2.iconWidth()/2), p2.getY()+(p2.iconHeight()/2)));  
 p1.draw();
 p2.draw();
 
}
}

void pressedKey()
{
  if(key == CODED)
  {
   if(keyCode == LEFT)
   p2.setDirection(2);
   else if(keyCode == RIGHT)
  p2.setDirection(1); 
   else if(keyCode == UP)
  p2.setDirection(0); 
   else if(keyCode == DOWN)
 p2.setDirection(3);  
  }
}
