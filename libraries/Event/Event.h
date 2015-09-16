/*
  Event
  
  This class serves as a "callback" manager to register events
  to happen on certain triggers or after certain intervals.
 */ 
 
#ifndef EVENT_H
#define EVENT_H

#include <Arduino.h>
#include <Configurations.h>
#include <SdFile.h>
#include <Rule.h>
#include <NilRTOS.h>
/**
 * EventTask is a structure that serves as an
 * abstract class of a "dispatchable" object.
 */
struct EventTask
{
    virtual void execute() = 0;   
};

/**
 * The Subscriber is the object that
 * encapsulates the Event it's listening for
 * and the EventTask to be executed.
 */

class Subscriber
{
    public:
        Subscriber();
        Subscriber(uint8_t Label, uint16_t EType);
       
        uint8_t label; //Specifies which sensor
        uint16_t EType; //The event type is used when a certain sensor has more than one useable event
        uint16_t ruleOffset; //The offset specifies where the rule is saved on the sd card

        //Offset in the rule datafile is used to get a specific rule
        //Offset count is used to maintain new rules.
        static uint16_t offSetCount;       
};



/**
 * The EventManager is responsible for gathering subscribers
 * and dispatching them when the requested Event is
 * triggered.
 */
class EventManager
{
  public:
    static EventManager* GetEventManager();
    ~EventManager()
    {
      instanceFlag = false;
    }
    void subscribe(Subscriber sub);
    bool trigger(uint8_t label, uint16_t EType);
    void OpenRuleFile();
  private:
    EventManager();
    static EventManager* em; // singleton
    static bool instanceFlag; // singleton
    Subscriber _sub[MAX_RULES]; 
    Rule r;
    uint8_t _subPos;
};

#endif