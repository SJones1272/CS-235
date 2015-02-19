
int power;
int range;
int sensor = A0;
int motor = 9;
int hitCount;

void setup(){
  pinMode(motor, OUTPUT);
  pinMode(sensor, INPUT);
  power = LOW;
  Serial.begin(9600);
  hitCount = 0;
}


void loop(){
 while(hitCount < 20){
  range = analogRead(sensor);
  
  
  Serial.println(range);
  if(range > 125){
   range = 0;
   hitCount++;
   Serial.println(hitCount);
  }
  else{
   range = 375;
   hitCount = 0;
  }
  
  analogWrite(motor, range);
 }
}
