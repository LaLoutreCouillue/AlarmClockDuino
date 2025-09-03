#ifndef ALARMSMANAGER_H
#define ALARMSMANAGER_H
#include "Arduino.h"

typedef struct{
  // string Name;
  uint8_t Minute; 
  uint8_t Hour;
  bool ActiveDay[7]; //day the alarm is active, 0 is Monday 6 is Sunday
  bool IsActive;
}Alarm;

class AlarmsManager {
  public:
    void Init();
    Alarm GetAlarm(uint8_t id);
    void ToggleAlarm(uint8_t id, bool value);
    void ModifyAlarm(uint8_t id, Alarm newAlarm);
  private:
    Alarm _alarms[3];
};

#endif