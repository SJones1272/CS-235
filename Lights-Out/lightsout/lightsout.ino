/* More to come... */

int lightState; //current byte value for the 8 leds.
int dataArray[8]; //stores the values for turning on/off specific bits

//for the shift register
int latchPin = 13;
int dataPin = 13;
int clockPin = 13;
int led9Pin = 13;
//current state of the board
boolean board[3][3];

byte currentState;
void setup(){
  // 0xFF -- 11111111
  dataArray[0] = 0x01;  //00000001
  dataArray[1] = 0x02;  //00000010
  dataArray[2] = 0x04;  //00000100
  dataArray[3] = 0x08;  //00001000
  dataArray[4] = 0x10;  //00010000
  dataArray[5] = 0x20;  //00100000
  dataArray[6] = 0x40;  //01000000
  dataArray[7] = 0x80;  //10000000
  
}


void loop(){
  
  
}

void setLED(int ledToSet, boolean state)
  {
    digitalWrite(latchPin, 0);
    if(ledToSet == 9)
    {
      digitalWrite(led9Pin,state);
    }else if(state)
    {
      lightState = lightState | dataArray[ledToSet-1];
    }
    else {
      lightState = lightState & (~ dataArray[ledToSet-1]); //bitwise not is ~
    }
    
  //  shiftOut(dataPin, clockPin, lightState);
    digitalWrite(latchPin,1);
  }
  
void startGame(){
  //turns on a random number of lights - need to check to see if all combinations are winable
  currentState = dataArray[random(0,7)];
}

//won if all lights are off
boolean hasWon(){
  int sum = 0;
  for(int i = 0; i<3; i++)
  {
    for(int j = 0; j<3; j++){
       if(board[i][j]){
         break;}
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
}


//changes board based off of a given light position
void makeMove(int i){
  int rw = getRow(i);
  int cw = getCol(i);
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
       board[rw+1][cw] = !board[rw+1][cw];
    }
    //change down one position
    if(rw==1 || rw == 0){
      board[rw-1][cw] = !board[rw-1][cw]; 
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

//sends appropriate info to turn leds on/off based on current board
void updateBoard(){
  //use previous function
}
