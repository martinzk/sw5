#include "DummyLed.h"
#include "Arduino.h"

LED::LED()
{
}

LED::LED(int outPin)
{
  this->outPin = outPin;
  this->ledState = LOW;
  this->setup();
}

void LED::setup() 
{  
  pinMode(outPin, OUTPUT);
}

void LED::ToggleLED()
{
	ledState = !ledState;
	digitalWrite(outPin, ledState);
}

void LED::TurnOn()
{
	digitalWrite(outPin, HIGH);
}

void LED::TurnOff()
{
	digitalWrite(outPin, LOW);
}