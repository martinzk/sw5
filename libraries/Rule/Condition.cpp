#include <Condition.h>

Condition::Condition()
{
	this->state = 0;
}

uint8_t Condition::GetState()
{
	return this->state;
}

void Condition::SetState(uint8_t value)
{
	this->state = value;
}