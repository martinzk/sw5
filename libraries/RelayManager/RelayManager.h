#ifndef RELAYMANAGER_H
#define RELAYMANAGER_H

#include "Arduino.h"
#include <Relay.h>
#include <IStateComponent.h>
#include <StateContainer.h>
#include <Configurations.h>

class RelayManager
{
	public:
		static RelayManager* GetRelayManager();
		~RelayManager()
    	{
        	instanceFlag = false;
    	}
		void AddRelay(Relay relay);
		Relay *GetRelayPointerFromID(uint8_t ID);
		void Listen();
		uint8_t Size();
	private:
		uint8_t index;
		Relay relays[NUMBER_OF_RELAYS];

		RelayManager();
		static RelayManager* rm; // singleton
		static bool instanceFlag; // singleton
};
#endif