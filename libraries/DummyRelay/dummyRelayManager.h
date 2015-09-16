#ifndef RELAYMANAGER_H
#define RELAYMANAGER_H

#include "DummyRelay.h"
#include <Configurations.h>

class RelayManager {
  public:
	RelayManager();
	void Add(Relay);
	void Run();
  private:
    Relay relays[NUMBER_OF_RELAYS];
    int index;
};

#endif
  