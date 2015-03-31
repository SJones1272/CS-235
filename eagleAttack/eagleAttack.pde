import processing.serial.*;

Player p1, p2;
Serial myport;
String port = "/dev/tty.usbmodem1411";
int width = 500;
int controller = -1;
int height = 500;
int hitRange = 40;
boolean gameOver;
//PImage sprite_sheet = loadImage("squirrelSprites.png", "png");

void setup(){
  size(width, height);
  
  
  p1 = new Player(3,loadImage("http://vignette3.wikia.nocookie.net/sims/images/4/41/Squirrel.png/revision/latest?cb=20111022014337", "png"), 0,0,3);
  p2 = new Player(6,loadImage("eagle.png", "png"), width/2, height/2, 0);
  
  myport = new Serial(this, port, 9600); 
  gameOver = false;
  
 
  
}

int distance(int x1, int y1,int x2,int y2){
 return round(sqrt(sq(x1-x2) + sq(y1-y2))); 
}

void draw(){
  background(#000000);
 if(gameOver){
  
  
 }
else{
 pressedKey(); 
  if(myport.available() > 0)
  {
   controller = myport.read(); 
  p1.setDirection(controller);  
  }
  
 p1.update();
 p2.update();
 if(distance(p1.getX(), p1.getY(), p2.getX(), p2.getY()) <= hitRange)
   gameOver = true;
   
 println(distance(p1.getX(), p1.getY(), p2.getX(), p2.getY()));  
 scale(.4);
 p1.draw();
 scale(1);
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
