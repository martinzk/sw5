#include "RelayManager.h"

bool RelayManager::instanceFlag = false;
// singleton
RelayManager* RelayManager::rm = NULL;

RelayManager* RelayManager::GetRelayManager()
{
    if(!instanceFlag)
    {
        rm = new RelayManager();
        instanceFlag = true;
    }
    return rm; 
}

RelayManager::RelayManager()
{
    this->index = 0;
}

uint8_t RelayManager::Size()
{
    return this->index;
}


void RelayManager::AddRelay(Relay relay)
{
    //Todo error handling..
    if(index < NUMBER_OF_RELAYS)
    {
        this->relays[index] = relay;
        index++;
    }
}

Relay * RelayManager::GetRelayPointerFromID(uint8_t ID)
{
    for (int i = 0; i < this->Size(); ++i)
    {
        if(relays[i].GetID() == ID)
        {
            return &relays[i];
        }
    }
}

// check for each relay if it needs to be turned on/off
void RelayManager::Listen()
{
    for(int i = 0; i < this->Size(); i++)
    {   
        this->relays[i].Toggle();
    }
}