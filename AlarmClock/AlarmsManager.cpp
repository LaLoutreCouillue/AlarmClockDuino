#include "AlarmsManager.h"

void AlarmsManager::Init() {
  
}

Alarm AlarmsManager::GetAlarm(uint8_t id) {
  return _alarms[id];
}

void AlarmsManager::ToggleAlarm(uint8_t id, bool value){
  _alarms[id].IsActive = value;
}

void AlarmsManager::ModifyAlarm(uint8_t id, Alarm newAlarm){
  _alarms[id] = newAlarm;
}