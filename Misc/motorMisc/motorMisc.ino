int cycle;

void setup(){
  pinMode(9, OUTPUT);
  pinMode(A0, INPUT);
  cycle = 50;
  Serial.begin(9600);
}

void loop() {
  //cycle = analogRead(A0);
  //Serial.println(cycle);
  Serial.println(analogRead(A0));
  cycle = map(analogRead(A0), 0,1023, 0, 256);
  digitalWrite(9, HIGH);
  delay(cycle);
  digitalWrite(9, LOW);
  delay(cycle);

}
