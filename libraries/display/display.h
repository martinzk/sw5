#ifndef ____display__
#define ____display__

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Configurations.h>
#include <rtc.h>

class Display 
{
public:
	static Display* GetDisplay();
	~Display()
    {
        instanceFlag = false;
    }
    void PrintDate(DateTime);
    void PrintMemory(long);
    void Clear();
private:
    Display();
    static bool instanceFlag;
    static Display* display;
    void updateCurrentRow();
    void isAboveNine(uint8_t);
    uint8_t CurrentRow;
};

#endif 