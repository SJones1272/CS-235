import processing.serial.*;

int x,y,z,radius,r,g,b;
int UP_CODE = 100;
int LEFT_CODE = 102;
int RIGHT_CODE = 101;
int DOWN_CODE = 103;
int lr = 0;
Serial myPort;

void setup()
{
  x = 1;
  y = 0; r = (int) random(255);
  g = (int) random(255); b = (int) random(255);
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
 if(lr == LEFT_CODE){ //check for button L
  x--; z=x;
 }else if(lr == RIGHT_CODE){ //check for button R
 x++; z=x;
 }else if(lr == UP_CODE){
 y++; z=y;
}else if(lr == DOWN_CODE){
 y--; z=y;}

  
  popMatrix();
  fill(r,g,b);
  background(204);
 // camera(x, 35.0, 120, 50, 50, 0.0, 
   //      0.0, 1.0, 0.0);
       //  camera(cos(radians(x))*radius+50, 35.0, sin(radians(x))*radius, 50, 50, 0, 
         //0.0, 1.0, 0.0);
         float chgx = sin(radians(x))*radius+50;
         float chgy = sin(radians(y))*radius+50;
         if(x==z)
           chgy = 50;
         else
         chgx = 50;
         camera(chgx, chgy,(cos(radians(z)))*radius, 50, 50, 0, 
         0.0, 1.0, 0.0);
         pushMatrix();
  translate(50, 50, 0);
  rotateY(PI/3);
  box(45); 
}




