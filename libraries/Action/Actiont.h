#ifndef ACTIONT_H
#define ACTIONT_H


#include <IStateComponent.h>

class Action
{
	public:
		Action();
		Action(uint8_t state, uint16_t wait, uint16_t duration);
		IStateComponent* StateComponent;
		uint8_t GetDesiredState();
		uint16_t GetWait();
		uint16_t GetDuration();

	private:
		uint8_t desiredState;
		uint16_t wait, duration;
};
#endif