#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>
#include <IStateComponent.h>
#include <GapContainer.h>


class Alarm : public IStateComponent
{
    public:
    	Alarm();
        Alarm(uint8_t ID, uint8_t inPin);
        void Insert(uint8_t state, uint32_t waitSeconds, uint32_t durationSeconds);
        void SetState();
        uint8_t GetCurrentState();
        uint8_t GetID();
    private:
        GapContainer gc; 
	    uint8_t id, inPin, currentState;
	    void toggleAlarm();
};
#endif