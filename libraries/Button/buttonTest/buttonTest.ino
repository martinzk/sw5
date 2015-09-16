#include "Arduino.h"
#include <Button.h>
#include <RelayManager.h>
#include <Relay.h>
#include <Event.h>
#include <IEventComponent.h>
#include <Configurations.h>
#include <IStateComponent.h>
#include <StateContainer.h>
#include <Rule.h>
#include <Condition.h>
#include <Actiont.h>
#include <RelayTime.h>
#include <rtc.h>
#include <Wire.h>

Button butt(1, 23);

void setup()
{
	Serial.begin(9600);	
}

void loop()
{
	butt.Listen();
}