int cycle;

void setup(){
  pinMode(9, OUTPUT);
  pinMode(10, INPUT);
  cycle = 50;
  Serial.begin(9600);
}

void loop(){
  //cycle = analogRead(10);
  Serial.println(cycle);
  cycle = map(analogRead(10), 0,1023 - 289, 0, 50);
  digitalWrite(9, HIGH);
  delay(cycle);
  digitalWrite(9, LOW);
  delay(cycle);
}​
