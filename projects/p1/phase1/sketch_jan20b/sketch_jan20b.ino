/*
* CSC 460 Project 1 Phase 1
* Simon Diemert and Scott Low
* 
* This code is meant to read an analog joystick, and control a servo motor.
* Upon clicking the input button a signal is sent via an IR emitter.
* The signal being sent is a bit array that is specified at the top of this file. 
*/

int bitArray[10] = {1,0,0,1,0,0,0,0,1,0}; //bit array to be sent
int bitIndex = 0; //next value to be sent
int buttonState = 0; //state variable, high when the button has been pressed. 

int buttonPin = A2; //input to read the joystick switch
int buttonValue = 1000; 
int buttonPressed = 0;

int xvalue = 0;  //variable to store the xvalue of the joystick
int servoOutput = 90; //value to write to servo (PWM)
int servoInterval = 0; //value to adjsut the servo by (PWM)
int analogServoPinX = A0;

void setup()
{  
    Serial.begin(9600); 
    pinMode(13, OUTPUT); //IR signal output
    pinMode(6, OUTPUT); //Debug output
    pinMode(12, OUTPUT); //Debug output
    pinMode(7, OUTPUT); //Debug output
    pinMode(8, OUTPUT); //Motor PWM output
    
    //PWM SETUP

    //Clear timer config
    TCCR1A = 0;
    TCCR1B = 0;

    //Set to Fast PWM (mode 15)
    TCCR1A |= (1<<WGM10) | (1<<WGM11);
    TCCR1B |= (1<<WGM12) | (1<<WGM13);
    
    //No prescaler
    TCCR1B |= (1<<CS10);

    OCR1A = 421;  //50 us period
    OCR1C = 220;  //Target
    
    //500us timer setup
    TCCR3A = 0;
    TCCR3B = 0;

    TCCR3B |= (1 << WGM32); //set to CTC mode 4

    TCCR3B |= (1<<CS32); //set prescaler 256
    
    OCR3A = 31;   //set value for compare on timer 3

    TIMSK3 |= (1<<OCIE3A); //enable interrupt
    TCNT3=0;               //reset timer

    //Configure PWM Servo Driver
    
    //Clear timer config
    TCCR4A = 0;
    TCCR4B = 0;

    //Set to Fast PWM (mode 15)
    TCCR4A |= (1<<WGM40) | (1<<WGM41);
    TCCR4B |= (1<<WGM42) | (1<<WGM43);

    TCCR4A |= (1<<COM4C1);  //Enable output C.

    TCCR4B |= (1<<CS42);  //256 prescaler

    OCR4A = 1250;  //50 us period
    OCR4C = 70;    //Target (63 to 120 is safe range)
    TCNT4=0;       //reset timer

    //analog stick and servo setup
    pinMode(buttonPin, INPUT);	
    pinMode(analogServoPinX, INPUT);

 
}

/*
* Interrupt every 500us, 
*/
ISR(TIMER3_COMPA_vect){
  digitalWrite(12, !digitalRead(12));
  digitalWrite(7, buttonState);
  
  if(buttonState == 1 && bitIndex < 10){
    if(bitArray[bitIndex] == 1){
      //turn on PWM 
      TCCR1A |= (1<<COM1C1);
    }else{
      //turn off PWM
        TCCR1A &= 0xF3; 
    }
    bitIndex++;
  }else{
    buttonState = 0;
    bitIndex = 0; 
    TCCR1A &= 0xF3;

  }
}

/*
* Main loop, repeats indefinitely. 
* Polls the state of the button and joystick, 
* writes appropriate values to the servo.
* 
* Adjusts state for sending IR signals.
*/
void loop()
{
  //read value of button
  buttonValue = analogRead(buttonPin);
  
  //check state of button to make sure we don't send to many signals.
  if(buttonPressed == 0 && buttonState == 0){
    if(buttonValue < 10){
      buttonPressed = 1; 
      buttonState = 0; 
    }   
  }else{
    if(buttonValue > 10){
      buttonPressed = 0;
      buttonState = 1; 
    }
  }
 
 //read the value of the joystick
 xvalue = analogRead(analogServoPinX);
 
 //determine the interval to change the servo by
 if(xvalue < 515){
    servoInterval = -1;  
 }else if(xvalue > 550){
   servoInterval = 1;
 }else{
  servoInterval = 0; 
 }
 
 //set output value for servo
 servoOutput = servoOutput - servoInterval;
 if(servoOutput < 65){
   servoOutput = 65;  
   servoInterval = 0; 
 }
 
 //check to ensure that we are not overdriving the servo.
 if(servoOutput > 120){
  servoOutput = 120; 
  servoInterval = 0;
 }
 
 //Print debug values at end of loop. 
 Serial.print(servoOutput);
 Serial.print(" xvalue: ");
 Serial.print(xvalue);
 Serial.print(" intveral: ");
 Serial.println(servoInterval);
 
 //set PWM value for servo
 OCR4C = servoOutput;
}
