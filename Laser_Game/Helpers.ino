/* HELPER FUNCTIONS ***********************************************************************************/
void updatePhoto(){
  for(int i = 0; i< 8; i++)currentPhotoReads[i] = analogRead(mappedPins[i]);
}
void pushLasers(){
  for(int i = 0; i< 8; i++){
    digitalWrite(laserPins[i], laserStates[i]);
  }
} //send out the laserState values at the end of loop()
void clearLasers(){
  for(int i = 0; i < NUMLASERS; i++){
    setLaser(i,false);
  }
}
void darkLasers(){
  for(int i = 0; i < NUMLASERS; i++){
    writeLaser(i,LOW);
  }
}   //turn off all lasers without changing the laser state variables.
void setLaser(int i, boolean hilo){
  laserStates[i] = hilo;
}                 //update a laser state to be written at the end of loop()
void writeLaser(int i,int hilo){
  digitalWrite(laserPins[i],hilo);
}          //bypass the updating states to write a specific laser now
void stopLights(){
  pixels.clear();
}
void setAllPixels(uint32_t c){
  for(int i = 0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i,c);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}



void randomLasers(int incBy)
{
  int diff = 2 + incBy;
  Serial.println(incBy);
  if(diff > 8)
    diff = 8;
  boolean temp[] = {
    false,false,false,false,false,false,false,false  };

  int i = 0;
  while(i < diff){
    int r = random(8);
    if(!temp[r]){
      temp[r] = true;
      i++;
    }
  }

  for(int i = 0; i < 8; i++){ 
    Serial.print("Laser "); 
    Serial.print(i); 
    Serial.print(" is "); 
    if(temp[i]){
      Serial.println("ON"); 
    }
    else{
      Serial.println("OFF");
    }
    laserStates[i] = temp[i];
  }
}

boolean newRound(){
  return abc == 0;
}
boolean timeIsUp(){
  return currentTime - gameStartTime >= TIMELIMIT;
};
boolean laserOn(int i){
  return (basePhotoReads[i] - currentPhotoReads[i]) >= LASER_THRESHHOLD;
}
boolean isBlocked(int n){
  return laserStates[n] && !laserOn(n);
}
boolean selectionBlock(int i){
  return !laserOn(i);
}
boolean notBlocked(int i){
  return laserStates[i] && laserOn(i);
}
boolean anyBlocked() //should be called after updating photoresistor values.
{
  for(int i = 0; i < NUMLASERS; i++){
    if(isBlocked(i)){
      Serial.print("Laser "); 
      Serial.print(i); 
      Serial.println(" is Blocked");
      return true;
    }
  }
  return false; 
}
boolean allBlocked(){
  for(int i = 0; i < NUMLASERS; i++){
    if(notBlocked(i)){
      return false;
    }
  }
  return true;
}

