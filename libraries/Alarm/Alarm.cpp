#include "Alarm.h"

Alarm::Alarm(){}


Alarm::Alarm(uint8_t ID, uint8_t inPin)
{
	this->id = ID;
	this->inPin = inPin;
	this->currentState = 0;
	pinMode(inPin, OUTPUT);
}

void Alarm::Insert(uint8_t state, uint32_t waitSeconds, uint32_t durationSeconds)
{
	gc.AddGap(waitSeconds*1000, durationSeconds*1000, state); // add the gap to schedule
}    

void Alarm::SetState()
{
	//check if the state has changed.
	uint8_t state = gc.GetState();
	if (this->currentState != state)
	{
		//State has changed, toggle alarm.
		this->currentState = state;
		this->toggleAlarm();
	}

}

uint8_t Alarm::GetID()
{
	return this->id;
}


uint8_t Alarm::GetCurrentState()
{
	return this->currentState;
}

void Alarm::toggleAlarm()
{
	analogWrite(this->inPin, this->currentState*200);
}