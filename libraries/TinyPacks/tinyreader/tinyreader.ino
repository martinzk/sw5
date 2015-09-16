#include <TinyPacks.h>
#include "Rule.h"
#include <Event.h>
#include <Configurations.h>
#include <IEventComponent.h>
#include <Condition.h>
#include <Actiont.h>
#include <Button.h>
#include <RelayManager.h>
#include <IState.h>
#include <EventContainer.h>
#define MAX_PACKED_DATA 256
#define MAX_TEXT_LENGTH 32

PackReader reader;
EventContainer* ec = EventContainer::GetEventContainer();
StateContainer* sc = StateContainer::GetStateContainer();
//int ip = b.GetInputPin();

unsigned char packed_data[] = {0xdf, 0x00, 0x93, 0xff, 0x00, 0x90, 0x8a, 0x63, 0x6f, 0x6e, 0x64, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0xdf, 0x00, 0x21, 0xef, 0x85, 0x73, 0x74, 0x61, 0x74, 0x65, 0x41, 0x01, 0x84, 0x6e, 0x61, 0x6d, 0x65, 0x81, 0x44, 0xf0, 0x85, 0x73, 0x74, 0x61, 0x74, 0x65, 0x41, 0x01, 0x84, 0x6e, 0x61, 0x6d, 0x65, 0x82, 0x44, 0x32, 0x85, 0x65, 0x76, 0x65, 0x6e, 0x74, 0xff, 0x00, 0x2c, 0x88, 0x69, 0x6e, 0x70, 0x75, 0x74, 0x50, 0x69, 0x6e, 0x41, 0x2d, 0x89, 0x65, 0x76, 0x65, 0x6e, 0x74, 0x54, 0x79, 0x70, 0x65, 0x86, 0x53, 0x69, 0x6e, 0x67, 0x6c, 0x65, 0x8a, 0x62, 0x75, 0x74, 0x74, 0x6f, 0x6e, 0x4e, 0x61, 0x6d, 0x65, 0x84, 0x53, 0x74, 0x75, 0x65, 0x87, 0x61, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0xdf, 0x00, 0x21, 0xef, 0x85, 0x73, 0x74, 0x61, 0x74, 0x65, 0x41, 0x01, 0x84, 0x6e, 0x61, 0x6d, 0x65, 0x81, 0x44, 0xf0, 0x85, 0x73, 0x74, 0x61, 0x74, 0x65, 0x41, 0x01, 0x84, 0x6e, 0x61, 0x6d, 0x65, 0x82, 0x44, 0x32};
void setup()
{
	Serial.begin(9600);
	Serial.println(ip);
}
bool event = false;


char eventType[MAX_TEXT_LENGTH] = "";
uint8_t ID = 0;
uint8_t conditionState = 100;
int inputPin = 100;
int state = 100;

//print en masse
bool show_attr = true;

void loop()
{
	reader.setBuffer(packed_data, 223);
	reader.next();

	//Open the list of rules
	reader.openList();
	while(reader.next())
	{
		reader.openMap();
		Rule r;
		while(reader.next())
		{
			if(reader.match("event"))
			{
				deserializeEvent(&r);
			}
			else if(reader.match("conditions"))
			{
				deserializeCondition(&r);
			}
			else if(reader.match("actions"))
			{
				deserializeAction(&r);
			}
		}
		reader.close();
	}
	reader.close();
	if(show_attr)
	{
		Serial.print("buttonName: ");
		Serial.println(buttonName);
		Serial.print("inputPin: ");
		Serial.println(inputPin);
		Serial.print("eventType: ");
		Serial.println(eventType);
		Serial.println();
		Serial.print("Condition name: ");
		Serial.println(conditionName);
		Serial.print("Condition State: ");
		Serial.println(conditionState);
		Serial.println();
		Serial.print("ActionName: ");
		Serial.println(actionName);
		Serial.print("actionState: ");
		Serial.println(actionState);
		Serial.println("--------------------------------------");
	}
	delay(5000);
}

void deserializeEvent(Rule *r)
{
	reader.openMap();
	while(reader.next())
	{
		if(reader.match("ID"))
			ID = reader.getInteger();
		else if(reader.match("eventType"))
			reader.getString(eventType, MAX_TEXT_LENGTH);
	}
	reader.close();

	//Add the component
	r->Event = &(ec->GetEventComponent(ID));
	r->EventAttr = eventType;
}

void deserializeCondition(Rule *r)
{
	//Open list of conditions
	reader.openList();
	while(reader.next())
	{
		reader.openMap();
		while(reader.next())
		{
			if(reader.match("ID"))
				ID = reader.getInteger();	        
		    else if(reader.match("state"))
	 	 		conditionState = reader.getInteger(); 
		}
		reader.close();

		//Add the condition to the list of conditions
		Condition c;
		c.StateComponent = &(sc->GetStateComponent(ID));
		c.SetState(conditionState);
		r->AddCondition(c);
	}
	reader.close();
}

void deserializeAction(Rule *r)
{
	//Open list of actions
	reader.openList();
    while(reader.next())
    {
    	uint8_t ID = 0, state = 0; 
    	uint16_t delay = 0, duration = 0;
		reader.openMap();
		while(reader.next()) 
     	{
     	    if(reader.match("ID"))
     	    	ID = reader.getInteger();
     	    else if(reader.match("state"))
     	    	state = reader.getInteger();
     	    else if(reader.match("delay"))
     	    	delay = reader.getInteger();
     	    else if(reader.match("duration"))
     	    	duration = reader.getInteger();
     	}
     	reader.close();

     	//Add action to the rule

 		Action a(state, delay, duration);
 		a.StateComponent = &(sc->GetStateComponent(ID));
 		r->AddAction(a);

    }
    reader.close();
}