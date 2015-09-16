#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"
#include <Event.h>
#include <IEventComponent.h>
#include <Configurations.h>

class Button : public IEventComponent 
{
  public:
    Button();
    Button(uint8_t id, uint8_t inPin);
    void Listen();
    uint8_t GetID();
  private:
    // VARIABLES
    uint8_t lastReading, id, pushCounter, inPin;    
    uint32_t pushStartTime, lastDebounceTime;
    bool longPushRegistered;
    // METHODS
    void resetPush();
    void setAmountOfPushes();
    void executePush();
    bool pushing();
    PushType getPushType();
};
#endif