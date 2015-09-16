#ifndef RELAY_H
#define RELAY_H


#include "Arduino.h"
#include "DummyLed.h"
#include <Configurations.h>
#define MAX_NUMBER_OF_LEDS 3
#define NUMBER_OF_RELAYS 16

class Relay {
  public:
  	Relay();
    Relay(int inPin);
    void Read();
    void AddLED(LED);
    void Toggle();

	  LED led_list[MAX_NUMBER_OF_LEDS];
    int inPin, lastReading, index;
  private:
    void setup();
    bool toggled;
    unsigned long lastDebounceTime;
};

#endif
 
 