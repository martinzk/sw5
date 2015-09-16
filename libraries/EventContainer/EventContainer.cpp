#include <EventContainer.h>

bool EventContainer::instanceFlag = false;
// singleton
EventContainer* EventContainer::ec = NULL;

EventContainer* EventContainer::GetEventContainer()
{
	if(!instanceFlag)
	{
		ec = new EventContainer();
		instanceFlag = true;
	}
	return ec; 
}

EventContainer::EventContainer()
{
	this->bIndex = 0,  mIndex = 0, sIndex = 0;
	rtc = RTC::GetRTC();
}


void EventContainer::AddButton(Button b)
{
	if(bIndex < NUMBER_OF_BUTTONS)
	{
		this->buttons[bIndex] = b;
		bIndex++;
	}
}

void EventContainer::AddMotionSensor(MotionSensor ms)
{
	if(mIndex < NUMBER_OF_MOTIONSENORS)
	{
		this->motions[mIndex] = ms;
		mIndex++;
	}
}

void EventContainer::AddSoundSensor(SoundSensor ss)
{
	if(mIndex < NUMBER_OF_SOUNDSENSORS)
	{
		this->soundSensors[sIndex] = ss;
		sIndex++;
	}
}


void EventContainer::ListenForButtons()
{
	for (int i = 0; i < bIndex; ++i)
	{
		this->buttons[i].Listen();
	}
}

void EventContainer::ListenForMotion()
{
	for (int i = 0; i < mIndex; ++i)
	{
		this->motions[i].Listen();
	}
}

void EventContainer::ListenForSoundSensor()
{	
	for (int i = 0; i < sIndex; ++i)
	{
		this->soundSensors[i].Listen();
	}
}

void EventContainer::ListenForClock()
{
	rtc->Listen();
}


