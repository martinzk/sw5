#ifndef MODE_H
#define MODE_H

#include <Arduino.h>
#include <IStateComponent.h>
#include <GapContainer.h>

class Mode : public IStateComponent 
{
    public:
        Mode(uint8_t ID);
        void Insert(uint8_t state, uint32_t waitSeconds, uint32_t durationSeconds);
        void SetState();
        uint8_t GetCurrentState();
        uint8_t GetID();

        Mode();
    private:
        GapContainer gc;
        uint8_t id, inPin, currentState;

};

#endif