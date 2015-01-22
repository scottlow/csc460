int bit_array[10] = {1,0,1,0,0,1,0,0,1,1};
int bit_index = 0;
int button_state = 0;

int buttonPin = A2; 
int buttonValue = 1000;
int buttonPressed = 0;

int xvalue = 0; 
int servoOutput = 90;
int servoInterval = 0;
int analogServoPinX = A0;

void setup()
{
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(7, OUTPUT); 
    pinMode(6, OUTPUT);
    pinMode(8, OUTPUT);   
    Serial.begin(9600); 
    //PWM SETUP

	
    //Clear timer config
    TCCR1A = 0;
    TCCR1B = 0;

    //Set to Fast PWM (mode 15)
    TCCR1A |= (1<<WGM10) | (1<<WGM11);
    TCCR1B |= (1<<WGM12) | (1<<WGM13);

    //Enable output C.
    //TCCR1A |= (1<<COM1C1);
    
    //No prescaler
    TCCR1B |= (1<<CS10);

    OCR1A = 421;  //50 us period
    OCR1C = 220;  //Target
    
    //500us timer setup
    TCCR3A = 0;
    TCCR3B = 0;

    TCCR3B |= (1 << WGM32); //set to CTC mode 4

    TCCR3B |= (1<<CS32); //set prescaler 256

    OCR3A = 31;

    TIMSK3 |= (1<<OCIE3A); //enable interrupt
    TCNT3=0; //reset timer

    //configure servo pwm
    //Clear timer config
    TCCR4A = 0;
    TCCR4B = 0;

    //Set to Fast PWM (mode 15)
    TCCR4A |= (1<<WGM40) | (1<<WGM41);
    TCCR4B |= (1<<WGM42) | (1<<WGM43);

    //Enable output C.
    TCCR4A |= (1<<COM4C1);
    //256 prescaler
    TCCR4B |= (1<<CS42);

    OCR4A = 1250;  //50 us period
    OCR4C = 70;  //Target (63 to 120 is safe range)
    TCNT4=0; //reset timer

    //analog stick and servo setup
    pinMode(buttonPin, INPUT);	
    pinMode(analogServoPinX, INPUT);

 
}

ISR(TIMER3_COMPA_vect){
  digitalWrite(12, !digitalRead(12));
  digitalWrite(7, button_state);
  
  if(button_state == 1 && bit_index < 10){
    if(bit_array[bit_index] == 1){
      //turn on PWM 
      TCCR1A |= (1<<COM1C1);
    }else{
      //turn off PWM
        TCCR1A &= 0xF3; 
    }
    bit_index++;
  }else{
    button_state = 0;
    bit_index = 0; 
    TCCR1A &= 0xF3;

  }
}

void loop()
{
  buttonValue = analogRead(buttonPin);
  //Serial.println(buttonValue);
  digitalWrite(6, buttonValue);
  if(buttonPressed == 0 && button_state == 0){
    if(buttonValue < 10){
      buttonPressed = 1; 
      button_state = 0; 
    }   
  }else{
    if(buttonValue > 10){
      buttonPressed = 0;
      button_state = 1; 
    }
  }
 
 xvalue = analogRead(analogServoPinX);
 
 if(xvalue < 515){
    servoInterval = -1;  
 }else if(xvalue > 550){
   servoInterval = 1;
 }else{
  servoInterval = 0; 
 }
 servoOutput = servoOutput - servoInterval;
 if(servoOutput < 65){
   servoOutput = 65;  
   servoInterval = 0; 
 }
 if(servoOutput > 120){
  servoOutput = 120; 
  servoInterval = 0;
 }
 Serial.print(servoOutput);
 Serial.print(" xvalue: ");
 Serial.print(xvalue);
 Serial.print(" intveral: ");
 Serial.println(servoInterval);
 OCR4C = servoOutput;
}
