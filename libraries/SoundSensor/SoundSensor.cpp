#include <SoundSensor.h>

SoundSensor::SoundSensor() 
{
	clapCounter = 0;
	clapStartTime = 0;
}

SoundSensor::SoundSensor(uint8_t id, uint8_t inPin)
{
	this->id = id;
	this->inPin = inPin;
	pinMode(inPin, INPUT);
	clapCounter = 0;
	clapStartTime = 0;
}


ClapType SoundSensor::GetClapType()
{
	if(!duringClap) // if we are not already reading a clap
	{
		uint32_t sumOfSquares = getSumOfSquares();
		averageReading = sumOfSquares/NUMBEROFSAMPLES; // get the average of the readings

		if(averageReading > THRESHOLD) // clap registered
		{
			this->duringClap = true; // we are dealing with a clap
	
			if (((millis() - clapStartTime) < SECOND_CLAP_MAX_INTERVAL) && clapCounter == 1) // if it's the second clap
			{
				this->clapStartTime = millis(); // when did the clap happend
				clapCounter++; // two claps
				return DOUBLECLAP;
			}
			this->clapStartTime = millis(); // when did the clap happend
			clapCounter = 1; // if not double clap then one clap
			return CLAP; // return the first clap
		}
	}
	else if((millis() - clapStartTime) > SOUND_SENSOR_DEBOUNCE_TIME)  // in this time we can not register more claps (debounce)
	{
		this->duringClap = false; // we have registered a clap and can now get a new one
		if(clapCounter == 2) // if we have registered two claps last time we will set clapCounter
		{					 // to 0 so we can register a new clap
			clapCounter = 0;
		}
	}
	return NOCLAP; // if it did not return any claps then NOCLAP is returned
}

uint32_t SoundSensor::getSumOfSquares()
{
	uint32_t sumOfSquares = 0;

	for(int i = 0; i < NUMBEROFSAMPLES; i++) // read NUMBEROFSAMPLES values
	{
	    sample = analogRead(inPin);
	    signal = (sample - MIDDLEVALUE); // substract the middlevalue to get the signal received
	    signal *= signal; // square it
	    sumOfSquares += signal; // add it to sum
	}
	return sumOfSquares;
}

uint8_t SoundSensor::GetID()
{
	return this->id;
}

void SoundSensor::Listen()
{

	if (GetClapType() == DOUBLECLAP)
	{
		Serial.println("dclap");
		EventManager* em = EventManager::GetEventManager();
		em->trigger(this->id, DOUBLECLAP);
	}
}	