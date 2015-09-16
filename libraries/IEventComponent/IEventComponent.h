//Interface for hardware and modes. Used to manage events in rules, every component needs to implement this.
//Every hardware component should be able to return its name and its state.

#ifndef IEVENTCOMPONENT_H
#define IEVENTCOMPONENT_H

#include <Arduino.h>

class IEventComponent {
	public:
		virtual uint8_t GetID() = 0;
		virtual void Listen() = 0;
};

#endif
