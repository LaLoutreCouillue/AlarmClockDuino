#ifndef ALARMEDITIONVIEW_H
#define ALARMEDITIONVIEW_H
#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "AlarmsManager.h"
#include "IView.h"
#include "NavigationHandler.h"
#include "ControlContext.h"

class AlarmEditionView : public IView{
  public:
    AlarmEditionView(AlarmsManager& am, NavigationHandler& nav);
    void Render(int width, int height, uint8_t param);
    void Update();
    void IncreaseTime(uint8_t id);
    void DecreaseTime(uint8_t id);
    void ToggleDay(uint8_t id, bool value);
    void SaveAlarm();
    void GoBack();
  private:
    ControlContext _controlContexts[13];
    void RefreshTime();
    uint8_t _alarmId;
	  Alarm _alarm;
    AlarmsManager& refToAm;
    NavigationHandler& refToNav;
    lv_obj_t* _hourLabel;
    lv_obj_t* _minuteLabel;
};
#endif