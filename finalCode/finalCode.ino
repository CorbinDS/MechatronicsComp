/*
 * Milestone 4 Code
 *
 * Created (11/28/2022): 
 * Author (Corbin Stickley - cds244, Jahnavi Sinha - js2747, Grace Hoedemaker - ghh58):
 */ 
#define MS_INCH_CONV_FW 120
#define MS_INCH_CONV_BW 166 
#define TURN_R 580
#define TURN_L 550 

float period = 0; 
bool blue;

ISR(PCINT2_vect){	
  if(PIND & 0b00001000){     
    TCNT1 = 0b0000000; //reset timer to 0
  } else {
    period = TCNT1; //store timer value in period
  }
}
void initColor(){
  PCICR  = 0b00000100; //enable pin change interrupt 2
  sei();
  TCCR1B = 0b00000001; //set prescaler to 1
  TCCR1A = 0b00000000; //set TIMER1 to normal mode
}
int getColor(){
	PCMSK2 = 0b00001000; //start checking on bit 3 for interrupt
  _delay_ms(10);
  PCMSK2 = 0b00000000; //stop checking on bit 7 for interrupt
  //adjust for period -> time. .0625 converts from clock ticks to s
  // *2 adjusts for half a period
  period = period * .0625 * 2;
}

bool checkBlack(){
  return period > 190.0;
}

bool checkBlue(){
  return (period > 140.0 && period < 180.0);
}

bool checkYellow(){
  return (period < 40.0);
}

bool checkOtherColor(){
  if (blue){
    return checkYellow();
  } else {
    // Serial.print(period);
    // Serial.print(" result of checkBlue");
    // Serial.println(checkBlue());
    return checkBlue();
  }
}

void initIO(){
  DDRD = 0b11110000; //motors 7-4, color in 3
}

void turnRight(){
  PORTD = 0b10010000;  //Set port 4 to high, port 5 to low, 7 to high, 6 to low (turn left)
  delay(TURN_R);      //continue turning for half a second
}

void smallTurnRight(){
  PORTD = 0b10010000;  //Set port 4 to high, port 5 to low, 7 to high, 6 to low (turn left)
  delay(80);      //continue turning for a little
}

void tinyTurnRight(){
  PORTD = 0b10010000;  //Set port 4 to high, port 5 to low, 7 to high, 6 to low (turn left)
  delay(40);      //continue turning for a little
}


void turnLeft(){
  PORTD = 0b01100000;  //Set port 4 to high, port 5 to low, 7 to high, 6 to low (turn right)
  delay(TURN_L);     //back for 1 second
}

void smallTurnLeft(){
  PORTD = 0b01100000;  //Set port 4 to high, port 5 to low, 7 to high, 6 to low (turn left)
  delay(80);      //continue turning for a little
}

void tinyTurnLeft(){
  PORTD = 0b01100000;  //Set port 4 to high, port 5 to low, 7 to high, 6 to low (turn left)
  delay(40);      //continue turning for a little
}

void turnAwayFromY(){
  if (!blue){
    smallTurnLeft();
  } else {
    smallTurnRight();
  }
  delay(20);
}

void turnAwayFromB(){
  if (!blue){
    smallTurnRight();
  } else {
    smallTurnLeft();
  }
  delay(20);
}

void forward(float inches){
  PORTD = 0b01010000;  //Set port 5 to high, port 4 to low (forward)
  delay(inches*MS_INCH_CONV_FW);     //Forward for 1 second
}

void backward(float inches){
  PORTD = 0b10100000;  //Set port 5 to high, port 4 to low (forward)
  delay(inches*MS_INCH_CONV_BW);     //Forward for 1 second
}

void stop(){
  PORTD = 0b00000000;
  delay(6000);
}

void forwardUntilBlack(){
  PORTD = 0b01010000;
  while(checkBlack() == false){
    getColor();
  }  
}

void forwardUntilMid(){
  int consistent = 0;
  PORTD = 0b01010000;  
  while (consistent < 10){
    getColor();
    if (checkOtherColor() == true){
      consistent++;
    } else { 
      consistent = 0;
    }
    while(checkBlack()){
      backward(4.0);
      smallTurnLeft();
      getColor();
    }
    PORTD = 0b01010000;  
  }
}

void forwardUntilMidOrBlack(){
  PORTD = 0b01010000;  
  getColor();
  bool result = checkBlack() || checkOtherColor();
  while(result == false){
    getColor();
    result = checkBlack() || checkOtherColor();
  }
  if (checkBlack() == true){
    float starting_p = period;
    getColor();
    float leftDiff;
  } else {
    driveParallel();
  }
}

void driveParallel(){
  PORTD = 0b01010000;  
  int turnedCount = 0;
  getColor();
  while(checkBlack() == false){
    //If we are straddling the midline and our period drops below 30
    //we are too far in the yellow
    if (turnedCount < 0){
      if (period < 45){ 
        turnAwayFromY();
        turnedCount = 20;
      } else if (period > 120) { //If period is greater than 120, we are too far in the blue
        turnAwayFromB();
        turnedCount = 20;
      }
    }
    PORTD = 0b01010000;  
    turnedCount--;
    getColor();
  }
}

void initialize(){
  Serial.begin(9600);
  delay(2000);  
  initIO();
  initColor();  
  getColor();
  Serial.println(period);
  if (checkBlue()){
    blue = true;
  } else {
    blue = false;
  }
  Serial.println("finished setup");
}

void mainProgram() {
   //Go until we hit the right wall (assumed starting config is facing RIGHT)
  forwardUntilBlack();
  backward(1.0);
  
  //Turn towards the midline and go forward until we detect it
  turnRight();
  forwardUntilMid();
  forward(2.0);

  //Turn to left wall, drive parallel to midline
  turnRight();
  driveParallel();

  //deposit our blocks first
  turnRight();
  smallTurnRight();
  forward(12.0);

  //then go to enemy side
  //turning in a way that doesn't push cubes into enemy side
  smallTurnLeft();
  backward(17.0);
  turnLeft();
  turnLeft();
  turnLeft();

  //sweep the entire enemy side of the mid line
  //then turn to our side, and go forward
  forward(40.0);
  turnLeft();
  smallTurnLeft();
  smallTurnLeft();
  forward(24.0);
  

  //back up to deposit blocks safely in our side
  backward(9.0);
  turnLeft();
  turnLeft();

  //forwardUntilMidOrBlack();
}

int main(void){
  init();
  initialize();
  mainProgram();
  return 0;
}
