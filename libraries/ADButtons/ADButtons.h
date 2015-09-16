#ifndef ADBUTTONS_H
#define ADBUTTONS_H

#include "Arduino.h"



class ADButtons 
{
	public:
		ADButtons();
		ADButtons(uint8_t inPin);
		bool AddButton(uint8_t id, uint16_t analogOutput, uint16_t deviation);
		uint8_t RegisterPush();
		void SetInPin(uint8_t);
	private:
		class Button
		{
			public:
				Button(uint8_t id, uint16_t analogOutput, uint16_t deviation);
				uint8_t buttonId;
				uint16_t analogOutput, deviation
				uint32_t startTime;
				bool wasPushed;
				Button();
		};
		
		uint8_t inPin,  numButtons;
		uint32_t noPushStartTime;
		bool clicked;
		//bool isPushed(Button *b, int reading);
		Button buttons[MAX_AD_BUTTONS]; 
	};

#endif