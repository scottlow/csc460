// Do not remove the include below
#include "p1.h"


//The setup function is called once at startup of the sketch
void setup()
{
	pinMode(13, OUTPUT);
	pinMode(12, OUTPUT);
}

// The loop function is called in an endless loop
void loop()
{
	digitalWrite(13, !digitalRead(13));
	delay(500);
}
