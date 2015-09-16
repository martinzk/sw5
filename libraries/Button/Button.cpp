#include "Button.h"

// empty constructor
Button::Button() {
	this->pushStartTime = 0;
	this->lastDebounceTime = 0;
	this->pushCounter = 0;
	this->longPushRegistered = false;
}

Button::Button(uint8_t ID, uint8_t inPin)
{
	this->id = ID;
	this->inPin = inPin;
	pinMode(inPin, INPUT_PULLUP);
	this->pushStartTime = 0;
	this->lastDebounceTime = 0;
	this->pushCounter = 0;
	this->longPushRegistered = false;
}

uint8_t Button::GetID()
{
	return this->id;
}


// Register push and execute event if push found
void Button::Listen()
{
	if (this->pushing()) // if pushing
	{
		this->setAmountOfPushes(); // check if it is fist or second push
	}
	else // if we are not pushing we should see if we have made a push before and determine its type
	{
		if(this->pushStartTime != 0) // if push has been registered
		{
			this->executePush(); 
		}
	}	
}

bool Button::pushing()
{
	bool isPush = false;

	//Read state of button
	uint8_t reading = digitalRead(inPin);

	//If the button changed, because to noise or pressing
	if(reading != this->lastReading) 
	{
		this->lastDebounceTime = millis();
	}

	//Remove noise by only entering section when debounce time is met. 
	if(millis() - this->lastDebounceTime > DEBOUNCE_TIME) 
	{
		if (reading == LOW)
		{
			//Push is registered
			isPush = true;
		}
	}

	this->lastReading = reading;
	return isPush;
}

// first or second push
void Button::setAmountOfPushes()
{
	if(this->pushCounter == 0) // if no push has been registered
	{
		this->pushCounter = 1; // register a single push
		this->pushStartTime = millis(); // and the time it happened
	}
	// prior push was a LONG_PUSH and we have not registered second push yet
	else if(this->longPushRegistered == true && this->pushCounter == 1)
	{
		this->pushCounter++; // additional push
		this->pushStartTime = millis(); // time this push happened
	}
}

// if not pushing execute the push that was made
void Button::executePush()
{
	PushType pushType = this->getPushType(); // get push type
	if(pushType != NO_PUSH) // if a push has been registered
	{
		EventManager* em = EventManager::GetEventManager();
		em->trigger(this->id, pushType); 
	} 
}	

// returns the type of push that has been made
PushType Button::getPushType()
{
	if(this->pushCounter == 2) // if a push has been registered after a LONG_PUSH
	{
		this->resetPush(); // push has been performed
		return DOUBLE_PUSH;
	}
	// if no push has been registered after LONG_PUSH
	if((millis() - this->pushStartTime) > LONG_PUSH_DURATION)
	{
		this->resetPush(); // push has been performed
		return LONG_PUSH;
	}
	// if push was a LONG_PUSH
	if ((millis() - this->pushStartTime) > LONG_PUSH_START)
	{
		this->longPushRegistered = true; // long push is registered
		return NO_PUSH;
	}
	if(this->pushCounter == 1) // if it is first push and it was not a LONG_PUSH it was a SHORT_PUSH
	{
		this->resetPush(); // push has been performed
		return SHORT_PUSH;
	}
}

// sets variables so we can register a new push
void Button::resetPush()
{
	this->longPushRegistered = false; // not a LONG_PUSH
	this->pushStartTime = 0;
	this->pushCounter = 0;
}

