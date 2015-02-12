/* More to come... */
#include <Bounce2.h>

int lightState; //current byte value for the 8 leds.
int dataArray[8]; //stores the values for turning on/off specific bits

//for the shift register
int latchPin = 13;
int dataPin = 11;
int clockPin = 12;
//current state of the board
boolean board[3][3];

//Button logic variables
int buttonMax = 9;
int ledMax = 9;      // Leds 0-8 Use shift register, led9 uses ?????? FILL IN HERE ??? ??
int firstButton = 2; //buttons use pins 2-10
Bounce b[9]; // An array of debouncers for the 9 buttons
boolean led[9]; //an array of booleans to keep track of LED states
int led9Pin = A0;

byte currentState;
void setup(){
  Serial.begin(9600);
  // 0xFF -- 11111111
  dataArray[0] = 0x01;  //00000001
  dataArray[1] = 0x02;  //00000010
  dataArray[2] = 0x04;  //00000100
  dataArray[3] = 0x08;  //00001000
  dataArray[4] = 0x10;  //00010000
  dataArray[5] = 0x20;  //00100000
  dataArray[6] = 0x40;  //01000000
  dataArray[7] = 0x80;  //10000000
  
  //Initialize debouncers and button pins and set LED states to false
  for(int i = 0; i < buttonMax; i++)
  {
    b[i] = Bounce();
    pinMode(i+firstButton,INPUT);
    b[i].attach(i+firstButton);
    b[i].interval(10); 
  }
  //set the latch to output and LED9 to output
  pinMode(latchPin,OUTPUT);
  pinMode(led9Pin,OUTPUT); 
  while(true)
  {
   b[4].update();
   if(b[4].rose())
    {
     randomSeed(millis());
     break;
    }
  }
  startGame();
}


void loop(){
  
  //update all debouncers
  for(int i =0; i < buttonMax; i++){ b[i].update();}
  
  //check all debouncers
   for(int j = 0; j < buttonMax;j++)
  {
//    while(b[j].read()==LOW) {
//       digitalWrite(latchPin,0);
//       shiftOut(dataPin,clockPin,0xFF);
//       digitalWrite(led9Pin,HIGH);
//       digitalWrite(latchPin,1); 
//  }
    
   if(b[j].rose())
   {
          //Serial.print(j);    
    makeMove(j);
   }
  } 
  //update board
  updateBoard();
  //check if won
  if(hasWon())
  {
    while(true)
    {
       startGame();
       updateBoard();
       b[4].update();
       if(b[4].rose())
       {
         break;
       }
       delay(50);
    }
  }
}

void setLED(int ledToSet, boolean state)
  {
    digitalWrite(latchPin, 0);
    if(ledToSet == 8)
    {
      digitalWrite(led9Pin,state);
    }else if(state)
    {
      lightState = lightState | dataArray[ledToSet];
    }
    else {
      lightState = lightState & (~ dataArray[ledToSet]); //bitwise not is ~
    }
    digitalWrite(latchPin,1);
  }
  
void startGame(){
  //turns on a random number of lights - need to check to see if all combinations are winable
 for(int i = 0; i < 3;i++)
  {
   for(int j = 0; j < 3; j++)
  {
   board[i][j] = random(0,2);
  } 
  }
  board[random(0,2)][random(0,2)] = false;
   updateBoard();
//  setLED(8,true);
//  digitalWrite(latchPin,0);
//  shiftOut(dataPin,clockPin,0xFF);
//  digitalWrite(latchPin,1);
}

//won if all lights are off
boolean hasWon(){
  for(int i = 0; i<3; i++)
  {
    for(int j = 0; j<3; j++){
       if(board[i][j]){
         return false;
     }
    }
  }
  return true;
}

//finds row number
int getRow(int i){
  if((i==0) || (i==1) || (i==2))
  {
    return 0;
  }
  if((i==3) || (i==4) || (i==5))
  {
    return 1;
  }
  if((i==6) || (i==7) || (i==8))
  {
    return 2;
  }
}

int getCol(int i){
  if((i==0) || (i==3) || (i==6))
  {
    return 0;
  }
  if((i==1) || (i==4) || (i==7))
  {
    return 1;
  }
  if((i==2) || (i==5) || (i==8))
  {
    return 2;
  }
  else
    return -1;
    //Serial.println("Error in getCol" + "  " + i);
}


//changes board based off of a given light position
void makeMove(int i){                                              // [ 0 1 2]
  int rw = getRow(i);                                              // [ 3 4 5]
  int cw = getCol(i);                                              // [ 6 7 8]
  //if its a light in the first col
  if( cw == 0){ 
      //change starting position
       board[rw][cw] = !board[rw][cw];
       //change one over
       board[rw][cw+1] = !board[rw][cw+1];
      if((rw == 0) || (rw == 1)){
        //change one down
         board[rw+1][cw] = !board[rw+1][cw];
      }
      if((rw==1) || (rw==2)){
      //change one up
       board[rw-1][cw] = !board[rw-1][cw];
      }  
  }
  //if its a light in the middle col
  else if( cw == 1){
    //change starting position
    board[rw][cw] = !board[rw][cw];
    //change left and right position
    board[rw][cw+1] = !board[rw][cw+1];
    board[rw][cw-1] = !board[rw][cw-1];
    //change up one position
    if((rw==1) || (rw==2)){
       board[rw-1][cw] = !board[rw-1][cw];
    }
    //change down one position
    if(rw==1 || rw == 0){
      board[rw+1][cw] = !board[rw+1][cw]; 
    }
    
   }
  //if its a light in the last
  else{
    //change starting position
       board[rw][cw] = !board[rw][cw];
       //change one to left
       board[rw][cw-1] = !board[rw][cw-1];
      if((rw == 0) || (rw == 1)){
        //change one down
         board[rw+1][cw] = !board[rw+1][cw];
      }
      if((rw==1) || (rw==2)){
      //change one up
       board[rw-1][cw] = !board[rw-1][cw];
      }
  }
}

//HELPER FUNCTION from 
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}

//sends appropriate info to turn leds on/off based on current board
void updateBoard(){
  

   for(int k =0; k < 3; k++){
     for(int r = 0; r < 3; r++)
    {
     setLED((k*3+r),board[k][r]);
    } 
  }
  digitalWrite(latchPin,0);
  shiftOut(dataPin,clockPin,lightState);
  digitalWrite(latchPin,1);
}
