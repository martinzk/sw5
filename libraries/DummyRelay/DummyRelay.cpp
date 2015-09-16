#include "DummyRelay.h"
#define DEBOUNCE_TIME 10

Relay::Relay()
{
  this->index = 0;
}

Relay::Relay(int inPin)
{
  this->inPin = inPin;
  this->lastReading = HIGH;
  this->lastDebounceTime = 0;
  this->toggled = false;
  this->setup();
}


void Relay::AddLED(LED led) {
  this->led_list[index] = led;

  index++;
}

void Relay::Toggle()
{
  for(int i = 0; i < index; i++)
  {

      led_list[i].TurnOn();
  }
}

void Relay::Read() 
{
    int reading = digitalRead(inPin);

    //If the button changed, because to noise or pressing
    if(reading != lastReading) {
        lastDebounceTime = millis();
    }
    //Remove noise by only entering section when debounce time is met
    if(millis() - lastDebounceTime > 5) 
    {
        if (digitalRead(inPin) == LOW && !toggled)
        {
            
            for(int i = 0; i < index; i++)
            {

                led_list[i].ToggleLED();
            }
            this->toggled = true;
        }
        else if(digitalRead(inPin) == HIGH  && toggled)
        {
            this->toggled = false;
        }
    }   
    lastReading = reading;
}

void Relay::setup() {
  pinMode(inPin, INPUT_PULLUP);
}