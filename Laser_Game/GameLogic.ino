/* GAME LOGIC *****************************************************************************************/



void gameOver()
{
  pixels.clear();
  gameStartTime = millis();
  currentTime = gameStartTime;
  while(currentTime - gameStartTime <= RESETTIME){ 
    setAllPixels(pixels.Color(BRIGHTNESS,0,0));
    pixels.show();
    dodgeCount = 0;
    selecting = 0;
    lost = false; 
    abc = 0;
  }
  clearLasers();
  pushLasers();
  delay(500); 
}

void runPartyMode()
{ 
  Serial.println("Partying");
  uint16_t i, j;

  for(j=0; j<256*5; j++) { //5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    int curTime = millis();
    if((curTime % 50) == 0)
      writeLaser(partyStep,HIGH);
    else if((curTime % 50) == 49)
    {
      writeLaser(partyStep,LOW);
      partyStep++;
      if(partyStep >= 8)
        partyStep = 0;
    }
  }
}

void startTimeLights(boolean dodge){
  gameStartTime = millis();
  pixels.clear();
  if(dodge)
    setAllPixels(pixels.Color(0,BRIGHTNESS,0));
  else
    setAllPixels(pixels.Color(0,0,BRIGHTNESS));
  pixels.show();  
  delay(200);
}

void updateLights(boolean blue){
  int tempTime = currentTime - gameStartTime;
  tempTime = map(tempTime, 0, TIMELIMIT, 0, NUMPIXELS);
  pixels.clear();
  //Serial.println(tempTime);

  for(int i = 20; i>=(20-tempTime); i--){
    pixels.setPixelColor(i, pixels.Color(BRIGHTNESS,0,0));
  }
  if(!blue)
    for(int i = 0; i<=(20-tempTime); i++){
      pixels.setPixelColor(i, pixels.Color(0,BRIGHTNESS,0));
    }
  else
    for(int i = 0; i<=(20-tempTime); i++){
      pixels.setPixelColor(i, pixels.Color(0,0,BRIGHTNESS));
    }
  pixels.show();
}

void updateLightsUp(boolean blue){
  int tempTime = currentTime - gameStartTime;
  tempTime = map(tempTime, 0, TIMELIMIT, NUMPIXELS, 0);
  pixels.clear();
  //Serial.println(tempTime);

  for(int i = 20; i>=(20-tempTime); i--){
    pixels.setPixelColor(i, pixels.Color(BRIGHTNESS,0,0));
  }
  if(blue)
    for(int i = 0; i<=(20-tempTime); i++){
      pixels.setPixelColor(i, pixels.Color(0,0,BRIGHTNESS));
    }
  else
    for(int i = 0; i<=(20-tempTime); i++){
      pixels.setPixelColor(i, pixels.Color(0,BRIGHTNESS,0));
    }
  pixels.show();
}

void startRound(boolean dodge){
  randomLasers(floor(dodgeCount/2)); 
  pushLasers(); 
  startTimeLights(dodge);
}

void blinkLasers()
{
  int tempTime = currentTime - gameStartTime; 
  if((round(tempTime/1000) % 2) == 0) //if the remainder of tempTime/1000 is even, then turn lasers on, else turn them off
    pushLasers();
  else
    darkLasers();
}

void flashPixels(uint32_t c,int wait)
{
  int tempTime = currentTime - gameStartTime;
  int timeCheck = (round(tempTime/wait) % 3);
  if(timeCheck == 1 || timeCheck == 3){
    pixels.clear();
    pixels.show();
  }
  else{
    pixels.clear();
    setAllPixels(c);
    pixels.show();
  }
}

void animateGood(boolean dodge)
{
  if(dodge)
    flashPixels(pixels.Color(0,BRIGHTNESS,0),350);
  else
    flashPixels(pixels.Color(0,BRIGHTNESS,0),350);
}

void runDodgeGame(){
  currentTime = millis();
  if(newRound()){          //if a new round, generate random lasers, set the start time, and go
    startRound(true); 
    abc++; 
    Serial.println("Starting a new game!");
  }
  else if(abc == 1)      // show the player the lasers for TIMELIMIT secs and then proceed
  {
    updateLights(false);
    blinkLasers();
    if(timeIsUp()){ 
      Serial.println("Start dodging!");
      abc++; 
      gameStartTime = millis(); 
      pushLasers();
    }
  }
  else if(abc == 2)
  {// check if lasers are blocked, if not count up, else do nothing
    updatePhoto();
    //debugPhoto();
    if(anyBlocked()){
      lost = true;   //gameOver() will be called the next time loop initiates.
      return;
    }
    else{
      updateLightsUp(false); //otherwise count back up in green for TIMELIMIT secs
      if(timeIsUp()){ 
        Serial.println("Good job!");
        abc++; 
        gameStartTime = millis(); 
        clearLasers(); 
        pushLasers();
      }
    }
  }
  else if(abc == 3){
    lost = false; // make sure the lost variable is false.
    animateGood(true); //flash green pixels
    if(timeIsUp()){
      abc = 0; 
      dodgeCount++;
    } //start over with 1 more dodge
  }
  else{ //shouldn't happen
    Serial.println("INVALID GAME STATE");
    return;
  }  
}

void runBlockGame()
{
  currentTime = millis();
  if(newRound()){          //if a new round, generate random lasers, set the start time, and go
    startRound(false); 
    abc++; 
    Serial.println("Starting a new game!");
  }
  else if(abc == 1)      // show the player the lasers for TIMELIMIT secs and then proceed
  {
    updateLights(true);
    blinkLasers();
    if(timeIsUp()){ 
      Serial.println("Start blocking!");
      abc++; 
      gameStartTime = millis(); 
      pushLasers();
    }
  }
  else if(abc == 2)
  {// check if lasers are blocked, if not count up, else do nothing
    updatePhoto();
    //debugPhoto();
    if(!allBlocked()){
      lost = true;   //gameOver() will be called the next time loop initiates.
      return;
    }
    else{
      updateLightsUp(true); //otherwise count back up in green for TIMELIMIT secs
      if(timeIsUp()){ 
        Serial.println("Good job!");
        abc++; 
        gameStartTime = millis(); 
        clearLasers(); 
        pushLasers();
      }
    }
  }
  else if(abc == 3){
    lost = false; // make sure the lost variable is false.
    animateGood(false); //flash blue pixels
    if(timeIsUp()){
      abc = 0; 
      dodgeCount++;
    } //start over with 1 more dodge
  }
  else{ //shouldn't happen
    Serial.println("INVALID GAME STATE");
    return;
  }  
}

void runDodgeBlock(){
  if(newRound())
  {
    dodgeBlock = !dodgeBlock;
    startRound(dodgeBlock); 
    abc++;
  }
  if(dodgeBlock)
    runDodgeGame();
  else
    runBlockGame();
}
