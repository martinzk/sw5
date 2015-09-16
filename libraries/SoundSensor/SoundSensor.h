#ifndef SOUNDSENSOR_H
#define SOUNDSENSOR_H

#include "Arduino.h"
#include <Configurations.h>
#include <IEventComponent.h>
#include <Event.h>

class SoundSensor : public IEventComponent
{
	public:
		SoundSensor();
		SoundSensor(uint8_t id, uint8_t inPin);
		ClapType GetClapType();
		void Listen();
		uint8_t GetID();
	private:
		uint32_t getSumOfSquares();
		uint32_t clapStartTime; // when first clap is registered
		uint16_t signal; // the value once middlevalue has been subtracted
		uint16_t averageReading; // the average of the readings

		uint8_t clapCounter; // how many claps are registered
		uint16_t sample; // the value getting read from microphone
		bool duringClap;
		uint8_t inPin, id;
};

#endif