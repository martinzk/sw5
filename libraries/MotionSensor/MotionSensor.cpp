#include "MotionSensor.h"
  
MotionSensor::MotionSensor()
{

}


MotionSensor::MotionSensor(uint8_t ID, uint8_t inPin)
{
    this->inPin = inPin;
    this->id = ID;
    pinMode(inPin, INPUT);
}

void MotionSensor::Listen()
{
    if (digitalRead(this->inPin) == HIGH)
    {
        //Motion registered
        EventManager* em = EventManager::GetEventManager();
        em->trigger(this->id, 1); //0 because it only have one behaviour, motion or no motion.
    }
}

uint8_t MotionSensor::GetID()
{
    return this->id;
}