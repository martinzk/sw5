#include <Arduino.h>
#include <Button.h>

Button butt(1, 53);

void setup()
{
	Serial.begin(9600);	
}

void loop()
{
	butt.Listen();
}