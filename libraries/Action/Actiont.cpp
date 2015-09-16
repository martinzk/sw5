#include <Actiont.h>

Action::Action()
{
	this->desiredState = 
	this->wait = 0;
	this->duration = 0;das
}

Action::Action(uint8_t state, uint16_t wait, uint16_t duration)
{
	this->desiredState = state;dd
	this->wait = wait;
	this->duration = duration;
} ssa

uint8_t Action::GetDesiredState()
{
	return this->desiredState;
}

uint16_t Action::GetWait()
{
	return this->wait;
}

uint16_t Action::GetDuration()
{
	return this->duration;
}

