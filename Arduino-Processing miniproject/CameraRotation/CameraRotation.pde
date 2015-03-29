import processing.serial.*;

int x,y,state,MAX,radius;
int LEFT_CODE = 102;
int RIGHT_CODE = 101;
int lr = 0;
Serial myPort;

void setup()
{
  x = 1;
  y = 0;
  state =1;
  MAX = 360;
  radius = 100;
   size(1000, 500, OPENGL);
noFill();
background(204);
camera(70.0, 35.0, 120.0, 50.0, 50.0, 0.0, 
       0.0, 1.0, 0.0);
translate(50, 50, 0);
rotateX(-PI/6);
rotateY(PI/3);
box(45); 
pushMatrix();
  myPort = new Serial(this,"COM3",9600);
}

void draw()
{
      // updateX();
    if(myPort.available() > 0){
      lr = myPort.read();}
 if(lr == LEFT_CODE) //check for button L
  x--;
 else if(lr == RIGHT_CODE) //check for button R
 x++;

  
  popMatrix();
  noFill();
  background(204);
 // camera(x, 35.0, 120, 50, 50, 0.0, 
   //      0.0, 1.0, 0.0);
         camera(cos(radians(x))*radius+50, 35.0, sin(radians(x))*radius, 50, 50, 0, 
         0.0, 1.0, 0.0);
         pushMatrix();
  translate(50, 50, 0);
  rotateY(PI/3);
  box(45); 
}




void updateX()
{
  if(x>=MAX)
  state = 2;
  else if(x<-MAX)
  state = 1;
  
 if(state==1)
   x++;
  if(state==2)
   x--; 
}
