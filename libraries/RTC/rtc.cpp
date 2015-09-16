#include "rtc.h"
//

//Singleton
bool RTC::instanceFlag = false;
// singleton
RTC* RTC::rtc = NULL;

RTC* RTC::GetRTC()
{
	if(!instanceFlag)
	{
		rtc = new RTC();
		instanceFlag = true;
	}
	return rtc; 
}

RTC::RTC(){
	Wire.begin();
	this->id = 0;
}

void RTC::SetClock(uint8_t sec, 
	uint8_t min, 
	uint8_t hour, 
	uint8_t date,
	uint8_t month, 
	uint8_t year)
{
	
	this->clock.setSecond(sec);//Set the second 
	this->clock.setMinute(min);//Set the minute 
	this->clock.setHour(hour);  //Set the hour 
	this->clock.setDate(date);  //Set the date of the month
	this->clock.setMonth(month);  //Set the month of the year
	this->clock.setYear(year);  //Set the year (Last two digits of the year)
}

DateTime RTC::GetClock()
{
	DateTime dateTime;
	bool h12 = false, PM = false, century = false;
	nilSemWait(&i2cSem);
	dateTime.second = this->clock.getSecond();
 	dateTime.minute = this->clock.getMinute();
 	dateTime.hour = this->clock.getHour(h12, PM);
 	dateTime.date = this->clock.getDate();
 	dateTime.month = this->clock.getMonth(century);
 	dateTime.year = this->clock.getYear();
 	nilSemSignal(&i2cSem);
	return dateTime;
}


uint8_t RTC::GetID()
{
	return 0; //Special case for the rtc clock, since there only are one
}

void RTC::Listen()
{
	DateTime datetime = this->GetClock();
	EventManager* em = EventManager::GetEventManager();
	em->trigger(this->id, this->concatenate(datetime.hour, datetime.minute)); //trigger on the specific hour and miniut, e.g. 1222


}

uint16_t RTC::concatenate(uint16_t x, uint16_t y) {
    uint16_t pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;        	
}
