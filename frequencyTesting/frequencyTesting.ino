/*
 * Lab 4
 *
 * Created (11/3/2022): 
 * Author (Corbin Stickley, cds244):
 */ 
#define L_THRESHOLD 210
#define H_THRESHOLD 280
float period; 
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
void setup(){
  delay(5000);  
  initColor();  
  getColor();
  Serial.begin(9600);
  Serial.println(period);
  
}

void loop(){
  Serial.begin(9600);
    while (1){
      getColor();
      Serial.println(period);
    }
}