#include <TinyPacks.h>
#include <Rule.h>
#include <Event.h>
#include <Configurations.h>
#include <IEventComponent.h>
#include <Condition.h>
#include <Actiont.h>
#include <Button.h>
#include <RelayManager.h>
#include <IStateComponent.h>
#include <EventContainer.h>
#include <StateContainer.h>
#include <RuleContainer.h>


PackReader reader;
EventContainer* ec = EventContainer::GetEventContainer();
StateContainer* sc = StateContainer::GetStateContainer();
RuleContainer* rc = RuleContainer::GetRuleContainer();



unsigned char packed_data[] = {0xdf, 0x00, 0xc2, 0xff, 0x00, 0x5e, 0x8a, 0x63, 0x6f, 0x6e, 0x64, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0xcd, 0xec, 0x85, 0x73, 0x74, 0x61, 0x74, 0x65, 0x40, 0x82, 0x49, 0x44, 0x41, 0x01, 0x85, 0x65, 0x76, 0x65, 0x6e, 0x74, 0xf1, 0x89, 0x65, 0x76, 0x65, 0x6e, 0x74, 0x54, 0x79, 0x70, 0x65, 0x41, 0x01, 0x82, 0x49, 0x44, 0x41, 0x19, 0x87, 0x61, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0xdf, 0x00, 0x22, 0xff, 0x00, 0x1f, 0x85, 0x64, 0x65, 0x6c, 0x61, 0x79, 0x41, 0x05, 0x88, 0x64, 0x75, 0x72, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x40, 0x85, 0x73, 0x74, 0x61, 0x74, 0x65, 0x41, 0x01, 0x82, 0x49, 0x44, 0x41, 0x01, 0xff, 0x00, 0x5e, 0x8a, 0x63, 0x6f, 0x6e, 0x64, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0xce, 0xed, 0x85, 0x73, 0x74, 0x61, 0x74, 0x65, 0x41, 0x01, 0x82, 0x49, 0x44, 0x41, 0x02, 0x85, 0x65, 0x76, 0x65, 0x6e, 0x74, 0xf0, 0x89, 0x65, 0x76, 0x65, 0x6e, 0x74, 0x54, 0x79, 0x70, 0x65, 0x40, 0x82, 0x49, 0x44, 0x41, 0x21, 0x87, 0x61, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x73, 0xdf, 0x00, 0x22, 0xff, 0x00, 0x1f, 0x85, 0x64, 0x65, 0x6c, 0x61, 0x79, 0x40, 0x88, 0x64, 0x75, 0x72, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x41, 0x0a, 0x85, 0x73, 0x74, 0x61, 0x74, 0x65, 0x41, 0x01, 0x82, 0x49, 0x44, 0x41, 0x04};

void setup()
{
	Serial.begin(9600);
	DeserializeRules();
}

void loop()
{
}

void DeserializeRules()
{
	Serial.println("Deserialize");
	reader.setBuffer(packed_data, 197);
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
		
		rc->AddRule(r);
		reader.close();
	}
	reader.close();
	Serial.println("end");
	Serial.println("");
}

void deserializeEvent(Rule *r)
{
	uint8_t ID = 0, eventType = 0;
	reader.openMap();
	while(reader.next())
	{
		if(reader.match("ID"))
			ID = reader.getInteger();
		else if(reader.match("eventType"))
			eventType = reader.getInteger();
	}
	reader.close();

	//Add the component
	r->Event = ec->GetEventComponent(ID);
	r->EventAttr = eventType;
}

void deserializeCondition(Rule *r)
{
	//Open list of conditions
	reader.openList();
	while(reader.next())
	{
		uint8_t ID = 0, state = 0;
		reader.openMap();
		while(reader.next())
		{
			if(reader.match("ID"))
				ID = reader.getInteger();	        
		    else if(reader.match("state"))
	 	 		state = reader.getInteger(); 
		}
		reader.close();

		//Add the condition to the list of conditions
		Condition c;
		c.StateComponent = sc->GetStateComponent(ID);
		c.SetState(state);
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
 		a.StateComponent = sc->GetStateComponent(ID);
 		r->AddAction(a);

    }
    reader.close();
}