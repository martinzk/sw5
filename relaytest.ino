// datamanagerTest.ino
#include <Arduino.h>
#include <Configurations.h>
//#include <rtc.h>
#include <Wire.h>
#include <DS3231.h>
#include <SdFatUtil.h>
#include <Button.h>
#include <Relay.h>
#include <Event.h>
#include <TinyPacks.h>
#include <SdFat.h>
#include <SdFile.h>
#include <IEventComponent.h>
#include <IStateComponent.h>
#include <Rule.h>
#include <Condition.h>
#include <Actiont.h>
#include <GapContainer.h>
#include <EventContainer.h>
#include <RelayManager.h>
#include <StateContainer.h>
#include <DataManager.h>
#include <webserver.h>
#include <SPI.h>
#include <Ethernet.h>
#include <TinyPacks.h>
#include <mode.h>
#include <MotionSensor.h>
#include <Alarm.h>
#include <SoundSensor.h>
Webserver wb;
RelayManager* rm = RelayManager::GetRelayManager();
EventContainer* ec = EventContainer::GetEventContainer();
StateContainer* sc = StateContainer::GetStateContainer();
Relay relay(1, 40, 200);
Relay r2(2, 38, 200);
Relay r3(2, 42, 200);
Relay r4(2, 34, 200);
Relay r5(2, 32, 200);
Relay r6(2, 30, 200);
Relay r7(2, 28, 200);
Relay r8(2, 40, 200);
Relay r9(2, 42, 200);

void setup()
{
	Serial.begin(9600);
	relay.isReady = true;
	r2.isReady = true;
	r3.isReady = true;
	r4.isReady = true;
	r5.isReady = true;
	r6.isReady = true;
	r7.isReady = true;
	r8.isReady = true;
	r9.isReady = true;
	rm->AddRelay(relay);
	rm->AddRelay(r2);
	rm->AddRelay(r3);
	rm->AddRelay(r4);
	rm->AddRelay(r5);
	rm->AddRelay(r6);
	rm->AddRelay(r7);
	rm->AddRelay(r8);
	rm->AddRelay(r9);

	uint32_t t1 = 0, t2 = 0;

	for(int i = 0; i < 10000; i++)
	{
		t1 = micros();
		rm->Listen();
		t2 = micros() - t1;
		Serial.println(t2);
	}
}

void loop()
{
	rm->Listen();
}