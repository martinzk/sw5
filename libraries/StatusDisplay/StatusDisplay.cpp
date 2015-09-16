#include <StatusDisplay.h>


bool StatusDisplay::instanceFlag = false;
StatusDisplay* StatusDisplay::statusDisplay = NULL;

StatusDisplay* StatusDisplay::GetStatusDisplay()
{
	if (!instanceFlag)
	{
		statusDisplay = new StatusDisplay();
		instanceFlag = true;
		return statusDisplay;
	}
	return statusDisplay;
}


StatusDisplay::StatusDisplay()
{
	this->display = Display::GetDisplay();
	this->rtc = RTC::GetRTC();
	this->dateTime = this->rtc->GetClock();
	
}

void StatusDisplay::UpdateStatus()
{
	printDate();
	//printFreeMemory();
}

void StatusDisplay::printDate()
{
	DateTime pt = this->rtc->GetClock();
	nilSemWait(&i2cSem);
	this->display->PrintDate(pt);
	nilSemSignal(&i2cSem);
}

void StatusDisplay::printFreeMemory()
{
	nilSemWait(&i2cSem);
	this->display->PrintMemory(nilUnusedHeapIdle());
	nilSemSignal(&i2cSem);
}
