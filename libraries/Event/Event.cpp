/*
  Event
  
  This class serves as a "callback" manager to register events
  to happen on certain triggers or after certain intervals.
 */
 
#include "Event.h"



bool EventManager::instanceFlag = false;
// singleton
EventManager* EventManager::em = NULL;

EventManager* EventManager::GetEventManager()
{
    if(!instanceFlag)
    {
        em = new EventManager();
        instanceFlag = true;
    }
    return em; 
}

EventManager::EventManager()
{
    this->_subPos = 0;
}
//Initialization on static offsetcount to subscribers
uint16_t Subscriber::offSetCount = 0;

Subscriber::Subscriber() {}

Subscriber::Subscriber(uint8_t Label, uint16_t EType)
{
    this->label = Label;
    this->EType = EType;
    this->ruleOffset = offSetCount;
    this->offSetCount += sizeof(Rule);
}





/**
 * Subscribes a new Subscriber to the
 * event manager.
 */
void EventManager::subscribe(Subscriber sub)
{
    if(_subPos < MAX_RULES)
    {

        _sub[_subPos] = sub;
        _subPos++;

        Serial.print(F("Adding subscriber"));
        Serial.println(_sub[_subPos-1].label);
    }
}

void EventManager::OpenRuleFile()
{
    char buf[MAX_FILENAME_LENGTH] = {0};
    strcpy_P(buf, memRulesFile);

    if(!rulesFile.open(buf, O_READ))
    {
        Serial.println("Rule file doesnt exist");
      //sd.errorHalt("Error opening rules file in read mode"); 
      return;
    }

    rulesFile.seekSet(0);
    rulesFile.read(&r, sizeof(Rule));
}

/** 
 * Triggers a specified event which will find the applicable
 * Subscriber and execute it's EventTask
 */
bool EventManager::trigger(uint8_t label, uint16_t EType)
{
    bool isTriggered = false;
    
    nilSemWait(&SdSem);
    for (int i = 0; i < _subPos; i++) 
    {

        if (_sub[i].label == label && _sub[i].EType == EType)
        { 
            isTriggered = true;
            //Set the file to the rules offset
            rulesFile.seekSet(_sub[i].ruleOffset);

            //Read the rule from the 
            rulesFile.read(&r, sizeof(Rule));

            //Execute the rule.     
            r.execute();
        }  
    }
    nilSemSignal(&SdSem);
    return isTriggered;
}
