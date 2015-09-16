#include <Actiont.h>

Action::Action()
{
	this->desiredState = 0;
	this->wait = 0;
	this->duration = 0;
}

Action::Action(uint8_t state, uint32_t wait, uint32_t duration)
{
	this->desiredState = state;
	this->wait = wait;
	this->duration = duration;
}

uint8_t Action::GetDesiredState()
{
	return this->desiredState;
}

uint32_t Action::GetWait()
{
	return this->wait;
}

uint32_t Action::GetDuration()
{
	return this->duration;
}

