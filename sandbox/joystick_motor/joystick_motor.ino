/*
  Analog input, analog output, serial output

 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.

 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground

 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

// These constants won't change.  They're used to give names
// to the pins used:
#include <Servo.h>
Servo myservo; 
const int analogInPinX = A0;  // Analog input pin that the potentiometer is attached to
const int analogInPinY = A1;
//const int analogOutPin = 9; // Analog output pin that the LED is attached to
const  int buttonPin = A2; 

int yvalue = 0;        // value read from the pot
int xvalue = 0; 
int xvalue2 = 0;
int xvalue3  = 0; 
int buttonValue = 0; 
int outputValue = 0;        // value output to the PWM (analog out)
int scaled = 0;


void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  myservo.attach(9);
  myservo.write(0);
}

void loop() {
  // read the analog in value:
  xvalue3 = xvalue2; 
  xvalue2 = xvalue;
  xvalue = analogRead(analogInPinX);
  yvalue = analogRead(analogInPinY);  
  buttonValue = analogRead(buttonPin);

  // print the results to the serial monitor:
  Serial.print("x :" );
  Serial.print(xvalue);
  Serial.print(" y :" );
  Serial.print(yvalue);
   Serial.print(" button :" );
  Serial.print(buttonValue);
  Serial.println();
  
  scaled = (xvalue+xvalue2+xvalue3)/18;

  myservo.write(scaled);  
  Serial.print("servo: ");
  Serial.println(scaled);
  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(2);
}
