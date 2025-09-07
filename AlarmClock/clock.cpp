#include "mbed.h"
#include <mbed_mktime.h>
#include "Clock.h"
#include "Arduino.h"

void Clock::Init() {
  RTCset();
}

void Clock::RTCset()  // Set cpu RTC
{    
  tm t;
  t.tm_sec = (0);       // 0-59
  t.tm_min = (00);        // 0-59
  t.tm_hour = (21);         // 0-23
  t.tm_mday = (22);   // 1-31
  t.tm_mon = (07);       // 0-11  "0" = Jan, -1 
  t.tm_year = ((25)+100);   // year since 1900,  current year + 100 + 1900 = correct year
  set_time(mktime(&t));       // set RTC clock                                 
}

String Clock::GetTimeAsString()
{
    char buffer[32];
    tm t;
    _rtc_localtime(time(NULL), &t, RTC_4_YEAR_LEAP_YEAR_SUPPORT);
    strftime(buffer, 32, "%Y-%m-%d %k:%M:%S", &t);
    return String(buffer);
}

tm Clock::GetTime(){
  tm t;
  _rtc_localtime(time(NULL), &t, RTC_4_YEAR_LEAP_YEAR_SUPPORT);
  return t;
}

void Clock::SetTime(tm t){
  set_time(mktime(&t));       // set RTC clock  
}
