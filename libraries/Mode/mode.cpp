#include "mode.h"

Mode::Mode(){}


Mode::Mode(uint8_t ID)
{
	this->id = ID;
	this->currentState = 0;
}

void Mode::Insert(uint8_t state, uint32_t waitSeconds, uint32_t durationSeconds)
{
	gc.AddGap(waitSeconds*1000, durationSeconds*1000, state);
}

void Mode::SetState()
{
	//Get state from Mode time.
	//Check if the state has changed.
	uint8_t state = gc.GetState();
	if (this->currentState != state)
	{
		this->currentState  = state;
	}
}

uint8_t Mode::GetID()
{
	return this->id;
}


uint8_t Mode::GetCurrentState()
{

	return this->currentState;
}

