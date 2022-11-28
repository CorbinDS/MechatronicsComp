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
  return period > 320.0;
}

bool checkBlue(){
  return (period > 250.0 && period < 320.0);
}

bool checkYellow(){
  return (period > 20.0 && period < 50.0);
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
  delay(200);      //continue turning for half a second
}

void turnLeft(){
  PORTD = 0b01100000;  //Set port 4 to high, port 5 to low, 7 to high, 6 to low (turn right)
  delay(TURN_L);     //back for 1 second
}

void smallTurnLeft(){
  PORTD = 0b01100000;  //Set port 4 to high, port 5 to low, 7 to high, 6 to low (turn left)
  delay(80);      //continue turning for half a second
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

void setup(){
  delay(5000);  
  initIO();
  initColor();  
  getColor();
  Serial.begin(9600);
  Serial.println(period);
  if (checkBlue()){
    blue = true;
  } else {
    blue = false;
  }
}

void loop() {
  forwardUntilBlack();
  backward(3.0);
  turnLeft();
  forwardUntilMid();
  turnLeft();
  forwardUntilBlack();
  //deposit our blocks first
  turnLeft();
  forward(6.0);

  //then go to enemy side
  backward(9.0);
  turnRight();
  turnRight();
  turnRight();
  //sweep
  forwardUntilBlack();
  turnRight();
  forward(18.0);
}
