#include "ADButtons.h"

ADButtons::Button::Button(){}

ADButtons::Button::Button(uint8_t buttonId, uint16_t analogOutput, uint16_t deviation)
{
	this->buttonId = buttonId;
	this->analogOutput = analogOutput;
	this->deviation = deviation;
	this->startTime = 0;
	this->wasPushed = false;
}

ADButtons::ADButtons() 
{
  this->reading = 0;
  this->numButtons = 0;
  this->noPushStartTime = 0;
  this->noPushTime = 0;
}

void ADButtons::SetInPin(uint8_t inPin) 
{
  this->inPin = inPin;
  pinMode(inPin, INPUT);
}

ADButtons::ADButtons(uint8_t inPin) 
{
  this->inPin = inPin;
  this->reading = 0;
  this->numButtons = 0;
  this->noPushStartTime = 0;
  this->noPushTime = 0;
  this->clicked = false;
  pinMode(inPin, INPUT);
}

bool ADButtons::AddButton(uint8_t buttonId, uint16_t analogOutput, uint16_t deviation)
{

	if (numButtons < MAX_AD_BUTTONS && buttonId != 0) // zero for false
	{
		Button b(buttonId, analogOutput, deviation);
		buttons[numButtons] = b;
		numButtons++;
		return true;
	}
	else
	{
		return false;
	}
}

uint8_t ADButtons::RegisterPush()
{	
	uint16_t reading = analogRead(this->inPin);
	bool noPush = reading > 1009;
	
	if (noPush && noPushStartTime == 0)
	{
		noPushStartTime = millis();
	}
	else if(noPush)
	{
		//Ensure that the analog output stabilize at max.
		if(millis() - noPushStartTime > 200)
		{
			for (uint8_t i = 0; i < numButtons; i++)
			{
				buttons[i].startTime = 0;
				buttons[i].wasPushed = false;
			}
			noPushStartTime = 0;
			clicked = false;
		}
	}
	else if(!clicked && !noPush)
	{
		noPushStartTime = 0;
		for (int i = 0; i < numButtons; i++)
		{
			if (reading > (buttons[i].analogOutput - buttons[i].deviation) && reading < (buttons[i].analogOutput + buttons[i].deviation))
			{
				if(buttons[i].startTime == 0)
				{
					buttons[i].startTime = millis();
				}
				else if((millis() - buttons[i].startTime > PUSH_TIME) && !buttons[i].wasPushed)
				{
					buttons[i].wasPushed = true;
					clicked = true;
					return buttons[i].buttonId;				
				}
			}				
		}
	}
	return 0;
}