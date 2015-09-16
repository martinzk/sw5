#ifndef CONDITION_H
#define CONDITION_H

#include <IStateComponent.h>
#include <Arduino.h>
class Condition
{
	public:
		Condition();

		IStateComponent *StateComponent; //So you can get the state of the wanted hardware.
		uint8_t GetState();
		void SetState(uint8_t value);
	private:
		uint8_t state;
	
};
#endif