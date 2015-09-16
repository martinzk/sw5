#ifndef STATUSDISPLAY_H
#define STATUSDISPLAY_H
#include <rtc.h>
#include <NilRTOS.h>
#include <SdFatUtil.h>
#include <display.h>
#include <Arduino.h>

class StatusDisplay
{
public:
	static StatusDisplay* GetStatusDisplay();
	~StatusDisplay()
	{
		instanceFlag = false;
	};
	void UpdateStatus();
	void ButtonPush(uint8_t);
private:
	StatusDisplay();
	static bool instanceFlag;
	static StatusDisplay* statusDisplay;
	Display* display;
	RTC* rtc;
	DateTime dateTime;
	void printDate();
	void printFreeMemory();
};

#endif