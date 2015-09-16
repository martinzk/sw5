#include "Relay.h"

uint8_t Relay::activeRelays = 0;

Relay::Relay()
{
	setup();
}

Relay::Relay(uint8_t ID, uint8_t outPin, uint16_t relayTime)
{
	this->id = ID;
	this->outPin = outPin;
	this->relayTime = relayTime;
	pinMode(outPin, OUTPUT);
	digitalWrite(outPin, HIGH);
	setup();
}

void Relay::setup()
{
	this->relayStartTime = 0;
	this->isActive = false;
	this->currentState = 0; 
	this->isReady = false;
}

// update gaps
void Relay::Insert(uint8_t state, uint32_t waitSeconds, uint32_t durationSeconds)
{
	gc.AddGap(waitSeconds*1000, durationSeconds*1000, state); // add the gap to schedule
}    

// check if scheduled state is the same as current
void Relay::SetState()
{
	//Get state from relay time.
	//check if the state has changed.
	nilSemWait(&relaySem);
	uint8_t state = gc.GetState();
	if (this->currentState != state && !isActive)
	{
		this->currentState = state;	
		//State has changed, set the status of the relay to ready.
		this->isReady = !this->isReady;
	}
	nilSemSignal(&relaySem);
}

uint8_t Relay::GetID()
{ 
	return this->id;
}

uint8_t Relay::GetCurrentState()
{
	return this->currentState;
}

void Relay::SetAssumingState(uint8_t state)
{
	this->currentState = state;
}


// returns true if the relay has been turned on for more than relayTime
bool Relay::onTimeElapsed()
{
    //Check if the relay has exceeded its minimum on time.
    if((millis() - this->relayStartTime) > this->relayTime)
    {
        return true;
    }

    //Relay has to be on for a longer amount of time
    return false;
}

bool Relay::offTimeElapsed()
{
	    //Check if the relay has exceeded its minimum on time.
    if((millis() - this->relayStartTime) > OFF_TIME)
    {
        return true;
    }

    //Relay has to be on for a longer amount of time
    return false;
}

void Relay::Toggle()
{ 
	if (activeRelays < MAX_ACTIVE_RELAYS && !this->isActive && this->isReady && this->offTimeElapsed())
	{
		this->on();
	} else if (this->isActive && this->onTimeElapsed())
	{
		this->off();
	}
}

void Relay::on()
{
	nilSemWait(&relaySem);
	this->activeRelays++;
	this->isReady = !this->isReady;
	//Turn the relay on
	digitalWrite(outPin, LOW);
	this->isActive = true;
	//Take the time after the relay has been turned on
	this->relayStartTime = millis();
	nilSemSignal(&relaySem);
}

void Relay::off()
{
	this->activeRelays--;
    this->isActive = false;
	//Turn the relay off
	digitalWrite(outPin, HIGH);
	this->relayStartTime = millis();
}