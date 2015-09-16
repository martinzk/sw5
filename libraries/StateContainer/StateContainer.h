#ifndef STATECONTAINER_H
#define STATECONTAINER_H

#include <IStateComponent.h>
#include <Configurations.h>
#include <mode.h>
#include <Alarm.h>
//#include <rtc.h>

class StateContainer
{
	public:
		static StateContainer* GetStateContainer();
		~StateContainer()
    	{
        	instanceFlag = false;
    	}
		
		void AddStateComponent(IStateComponent *ic);
		void ListenForStateComponents();
		uint8_t GetLength();
		IStateComponent* GetStateComponent(uint8_t id);
		IStateComponent *stateComponents[NUMBER_OF_RELAYS + NUMBER_OF_MODES + NUMBER_OF_BUZZERS];

		void AddMode(Mode m);
		void AddAlarm(Alarm a);
	private:
		uint8_t index, modeindex, alarmIndex;
		Alarm alarms[NUMBER_OF_BUZZERS];
		StateContainer();
		Mode modes[NUMBER_OF_MODES];
		static StateContainer* sc; // singleton
		static bool instanceFlag; // singleton
};

#endif
