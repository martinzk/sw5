/*Interface for all components which has states, e.g. modes*/

#ifndef ISTATECOMPONENT_H
#define ISTATECOMPONENT_H

#include <Arduino.h>

class IStateComponent
{
  public:
    virtual void Insert(uint8_t state, uint32_t waitSeconds, uint32_t durationSeconds) = 0;
    virtual void SetState() = 0;
    virtual uint8_t GetCurrentState() = 0;
    virtual uint8_t GetID() = 0;
};

#endif
