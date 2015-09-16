#include <Arduino.h>
#include <DummyRelay.h>
#include <dummyRelayManager.h>
#include <DummyLed.h>



LED led1(22);  // KNAP I
LED led2(23);  // KNAP B
LED led3(24);  // KNAP C
LED led4(25);  // KNAP D
LED led5(26);  // KNAP E
LED led6(27);  // KNAP A
LED led7(28);  // KNAP B
LED led8(29);  // KNAP F
LED led9(30);  // KNAP G
LED led10(31); // KNAP H
LED led11(32); // KNAP J 
LED led12(33); // KNAP Iz
LED led13(34); // KNAP I
LED led14(35); // KNAP L
LED led15(36); // KNAP O
LED led16(37);  // KNAP P
LED led17(38);  // KNAP M
LED led18(39);  // KNAP N

Relay relay1(53);
Relay relay2(52);
Relay relay3(51); 
Relay relay4(50);
Relay relay6(49);
Relay relay5(48); 
Relay relay7(47);
Relay relay8(46);
Relay relay9(8);
Relay relay10(7);
Relay relay11(6);
Relay relay12(5);
Relay relay13(4);
Relay relay14(3);
Relay relay15(2);
Relay relay16(9);


RelayManager relayManager;


void setup()
{
  Serial.begin(9600);
  relay1.AddLED(led18);
  relay2.AddLED(led2);
  relay3.AddLED(led3);
  relay4.AddLED(led6);
  relay4.AddLED(led8);
  relay5.AddLED(led1);
  relay5.AddLED(led10);
  relay6.AddLED(led5);
  relay7.AddLED(led4);
  relay8.AddLED(led7);
  relay9.AddLED(led9);
  relay10.AddLED(led11);
  relay11.AddLED(led12);
  relay12.AddLED(led13);
  relay13.AddLED(led14);
  relay14.AddLED(led15);
  relay15.AddLED(led16);
  relay16.AddLED(led17);
  
  relayManager.Add(relay1);
  relayManager.Add(relay2);
  relayManager.Add(relay3);
  relayManager.Add(relay4);
  relayManager.Add(relay5);
  relayManager.Add(relay6);
  relayManager.Add(relay7);
  relayManager.Add(relay8);
  relayManager.Add(relay9);
  relayManager.Add(relay10);
  relayManager.Add(relay11);
  relayManager.Add(relay12);
  relayManager.Add(relay13);
  relayManager.Add(relay14);
  relayManager.Add(relay15);
  relayManager.Add(relay16);
  
  
}

void loop()
{
  
  relayManager.Run();
}