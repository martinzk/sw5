// datamanagerTest.ino
#include <Arduino.h>
#include <Configurations.h>
#include <rtc.h>
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
#include <mode.h>
#include <SPI.h>
#include <Ethernet.h>
#include <TinyPacks.h>
#include <MotionSensor.h>
#include <Alarm.h>
#include <SoundSensor.h>
#include <NilRTOS.h>
#include <StatusDisplay.h>
#include <display.h>
#include <avr_heap.h>
Webserver wb;
RelayManager* rm = RelayManager::GetRelayManager();
StateContainer* sc = StateContainer::GetStateContainer();
EventContainer* ec = EventContainer::GetEventContainer();
StatusDisplay* st;

NIL_WORKING_AREA(buttonThread, 128);
NIL_THREAD(Thread1, arg)
{
  while(true)
  {
      
      ec->ListenForButtons();
      nilThdSleepMilliseconds(35);
      
  }
}

NIL_WORKING_AREA(stateThread, 128);
NIL_THREAD(Thread2, arg)
{
    while(true)
    {
        sc->ListenForStateComponents();   
        nilThdSleepMilliseconds(50);
    }
}    

NIL_WORKING_AREA(relayThread, 64);
NIL_THREAD(Thread3, arg)
{
    while(true)
    {
        rm->Listen();  
        nilThdSleepMilliseconds(50);
    }
}

NIL_WORKING_AREA(waThread4, 64);
NIL_THREAD(Thread4, arg)
{
     while(true)
     {

         ec->ListenForSoundSensor();  
         nilThdSleepMilliseconds(50);
     }
}

NIL_WORKING_AREA(waThread5, 96);
NIL_THREAD(Thread5, arg)
{
    while(true)
    {
        ec->ListenForMotion();
        nilThdSleepMilliseconds(1000);
    }
}

NIL_WORKING_AREA(waThread6, 128);
NIL_THREAD(Thread6, arg)
{
    while(true)
    {
        st->UpdateStatus();
        nilThdSleepMilliseconds(1000);
    }
}

NIL_WORKING_AREA(wbThread, 256);
NIL_THREAD(getNewConfThread, arg)
{
    while(true)
    {
        wb.ListenForClients();
        nilThdSleepMilliseconds(2000);
    }
}

NIL_WORKING_AREA(waThread8, 128);
NIL_THREAD(Thread8, arg)
{
    while(true)
    {
        ec->ListenForClock();
        nilThdSleepMilliseconds(60000);
    }
}

NIL_THREADS_TABLE_BEGIN()
NIL_THREADS_TABLE_ENTRY(NULL, Thread1, NULL, buttonThread, sizeof(buttonThread))
NIL_THREADS_TABLE_ENTRY(NULL, Thread2, NULL, stateThread, sizeof(stateThread))
NIL_THREADS_TABLE_ENTRY(NULL, Thread3, NULL, relayThread, sizeof(relayThread))
NIL_THREADS_TABLE_ENTRY(NULL, Thread4, NULL, waThread4, sizeof(waThread4))
NIL_THREADS_TABLE_ENTRY(NULL, Thread5, NULL, waThread5, sizeof(waThread5))
NIL_THREADS_TABLE_ENTRY(NULL, Thread6, NULL, waThread6, sizeof(waThread6))
NIL_THREADS_TABLE_ENTRY(NULL, getNewConfThread, NULL, wbThread, sizeof(wbThread))
NIL_THREADS_TABLE_ENTRY(NULL, Thread8, NULL, waThread8, sizeof(waThread8))
NIL_THREADS_TABLE_END()

void setup() 
{
  Serial.begin(9600);
   WebserverSetup();
   delay(3000);
  DataManager dm;
  dm.GetDataFromSD();

  //Used to access rules fast.
  EventManager* em = EventManager::GetEventManager();
  em->OpenRuleFile();
  
  delay(1000);
  
  st = StatusDisplay::GetStatusDisplay();
  Serial.println(F("done"));
  delay(1000);
  
  nilSysBegin();  
}

void WebserverSetup()
{
   byte mac[] = { 
   0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED   };
   IPAddress ip( 192, 168, 1, 177 );
   IPAddress gateway( 192,168,1,1 );
   IPAddress subnet( 255,255,255,0 );
   wb.Init(ip, gateway, subnet, mac, 80);
}

void loop()
{
    
}
