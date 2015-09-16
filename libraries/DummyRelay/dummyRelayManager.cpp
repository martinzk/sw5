#include "dummyRelayManager.h"

RelayManager::RelayManager() {
	this->index = 0;
}

void RelayManager::Add(Relay relay) {
	this->relays[index] = relay;
	index++;
}
	
void RelayManager::Run() 
{
	Serial.println(index);
	for(int i = 0; i < index; i++) 
	{
            relays[i].Read();
	}
}
   