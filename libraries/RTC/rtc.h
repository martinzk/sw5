#ifndef RTC_H
#define RTC_H

#include <DS3231.h>
#include <Wire.h>
#include <NilRTOS.h>
#include <Arduino.h>
#include <Configurations.h>
#include <IEventComponent.h>
#include <Event.h>

typedef struct
{
    uint8_t second, minute, hour, date, month, year;
} DateTime;


class RTC {//: public IEventComponent {
  public:
    static RTC* GetRTC();
    ~RTC()
    {
        instanceFlag = false;
    }
    DateTime GetClock();
    uint8_t GetID();
    void Listen();
    void SetClock(uint8_t sec, 
    uint8_t min, 
    uint8_t hour, 
    uint8_t date,
    uint8_t month, 
    uint8_t year);
  private:
    RTC();

    int8_t previousMin;
    uint8_t id;
    DS3231 clock;
    uint16_t concatenate(uint16_t x, uint16_t y);
    //Singleton
    static RTC* rtc; // singleton
    static bool instanceFlag; // singleton
};

#endif