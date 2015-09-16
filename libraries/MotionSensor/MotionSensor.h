#ifndef MOTIONSENSOR_H
#define MOTIONSENSOR_H

#include <Configurations.h>
#include <IEventComponent.h>
#include <Event.h>

class MotionSensor : public IEventComponent
{
    public:
    	MotionSensor();
    	MotionSensor(uint8_t ID, uint8_t inPin);
    	uint8_t GetID();
    	void Listen();

    private:
	    uint8_t id, inPin;
};
#endif