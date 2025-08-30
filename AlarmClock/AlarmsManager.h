#ifndef ALARMSMANAGER_H
#define ALARMSMANAGER_H
#include "Arduino.h"

typedef struct{
  // string Name;
  uint8_t Second; 
  uint8_t Minute; 
  uint8_t Hour;
  bool ActiveDay[6]; //day the alarm is active, 0 is Monday 6 is Sunday
  bool IsActive;
}alarm;

class AlarmsManager {
  public:
    alarm Alarms[2];
    void Init();
    void AddAlarm();
};

#endif