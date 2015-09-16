// rtctest.ino
#include <rtc.h>
#include <Wire.h>


void setup() {
	
  Serial.begin(9600);
  DateTime t;
  uint16_t a = 0;
	RTC* asd;//.GetRTC();
  asd->SetClock((uint8_t)0,(uint8_t)40,(uint8_t)16,(uint8_t)7,(uint8_t)12,(uint8_t)14);
	//t = asd->GetClock();
	while(true){
  t = asd->GetClock();
  Serial.print(F("20"));
  Serial.print(t.year, DEC);
  Serial.print('-');
  Serial.print(t.month,DEC);
  Serial.print('-');
  Serial.print(t.date,DEC);
  Serial.print(' ');
  Serial.print(t.hour,DEC);
  Serial.print(':');
  Serial.print(t.minute,DEC);
  Serial.print(':');
  Serial.println(t.second,DEC);
  delay(1000);
  }
}


void loop() 
{

}