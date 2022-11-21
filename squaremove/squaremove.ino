/*
 * HW6 Q3
 *
 * Created (11/3/2022): 
 * Author (Corbin Stickley, cds244):
 */

//2000ms forward is ~43 cm = 1.4ft
//1450ms forward is pretty much exactly 1ft
//725 ms forward = ?
//440 ms left/right is ~90 deg
//120*18ms ~= 13 inches backwards
//
#define MS_INCH_CONV_FW 120
#define MS_INCH_CONV_BW 166 
#define TURN_R 530
#define TURN_L 460 

int fw_con1v;
int bw_con2v;
int counter = 0;


void turnRight(){
  PORTD = 0b10010000;  //Set port 4 to high, port 5 to low, 7 to high, 6 to low (turn left)
  delay(TURN_R);      //continue turning for half a second
}

void turnLeft(){
  PORTD = 0b01100000;  //Set port 4 to high, port 5 to low, 7 to high, 6 to low (turn right)
   delay(TURN_L);     //back for 1 second
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
  delay(4000);
}

void setup(){
  DDRD = 0b11110000;
  delay(10000);
}

void loop() {
  // if (counter == 0){
  //   forward(12.0);
  //   turnRight();
  //   forward(12.0);
  //   turnLeft();
  //   forward(6.0);
  //   backward(18.0);
  //   turnLeft();
  //   forward(12.0);
  //   PORTD = 0b00000000;
  //   counter++;
  // }
  forward(36.0);
  //turnRight();
  //turnRight();
}

