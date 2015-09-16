#ifndef EVENTCONTAINER_H
#define EVENTCONTAINER_H

#include <IEventComponent.h>
#include <Configurations.h>
#include <Button.h>
#include <Event.h>
#include <SoundSensor.h>
#include <MotionSensor.h>
#include <rtc.h>



class EventContainer
{
	public:
		static EventContainer* GetEventContainer();
		~EventContainer()
    	{
        	instanceFlag = false;
    	}
    	void AddButton(Button b);
    	void AddMotionSensor(MotionSensor ms);
    	void AddSoundSensor(SoundSensor ss);
    	void ListenForButtons();
    	void ListenForMotion();
    	void ListenForSoundSensor();
    	void ListenForClock(); 


	private:
		Button buttons[NUMBER_OF_BUTTONS];
		MotionSensor motions[NUMBER_OF_MOTIONSENORS];
		SoundSensor soundSensors[NUMBER_OF_SOUNDSENSORS];
		RTC *rtc;
		uint8_t bIndex, mIndex, sIndex;
		EventContainer();
		
		static EventContainer* ec; // singleton
		static bool instanceFlag; // singleton

};

#endif
