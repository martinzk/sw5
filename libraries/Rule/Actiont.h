#ifndef ACTIONT_H
#define ACTIONT_H


#include <IStateComponent.h>

class Action
{
	public:
		Action();
		Action(uint8_t state, uint32_t wait, uint32_t duration);
		IStateComponent* StateComponent;
		uint8_t GetDesiredState();
		uint32_t GetWait();
		uint32_t GetDuration();

	private:
		uint32_t wait, duration;
		uint8_t desiredState;
};
#endif