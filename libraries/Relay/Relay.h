#ifndef RELAY_H
#define RELAY_H

#include "Arduino.h"
#include <IStateComponent.h>
#include <GapContainer.h>
#include <Configurations.h>

class Relay : public IStateComponent
{
  public:
    // Constructors
    Relay();
    Relay(uint8_t ID, uint8_t outPin, uint16_t relayTime);

    //todo private.
    // Variables

    // Methods
    void Insert(uint8_t state, uint32_t waitSeconds, uint32_t durationSeconds); 
    void SetAssumingState(uint8_t state);
    void SetState();
    uint8_t GetID();
    uint8_t GetCurrentState();
    void Toggle();
    bool isReady;
  private:
    bool onTimeElapsed();
    bool offTimeElapsed();
    static uint8_t activeRelays;
    void on();
    void off();
    GapContainer gc; 
    uint8_t outPin;
    uint8_t id, currentState; 
    uint16_t relayTime;
    uint32_t relayStartTime;
    bool isActive;
    void setup();
};
#endif