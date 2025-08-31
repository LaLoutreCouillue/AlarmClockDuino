#ifndef TIMESETTINGVIEW_H
#define TIMESETTINGVIEW_H
#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "clock.h"
#include "IView.h"
#include "NavigationHandler.h"
#include "ButtonContext.h"

class TimeSettingView : public IView{
  public:
    TimeSettingView(Clock& c, NavigationHandler& nav);
    void Render(int width, int height);
    void Update();
    void IncreaseTime(uint8_t id);
    void DecreaseTime(uint8_t id);
    void SaveTime();
    void GoToMain();
  private:
    ButtonContext _buttonContexts[12];
    void RefreshTime();
    tm _time;
    Clock& refToC;
    NavigationHandler& refToNav;
    lv_obj_t* _dayLabel;
    lv_obj_t* _monthLabel;
    lv_obj_t* _yearLabel;
    lv_obj_t* _hourLabel;
    lv_obj_t* _minuteLabel;
};
#endif