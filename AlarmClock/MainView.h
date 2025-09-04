#ifndef MAINVIEW_H
#define MAINVIEW_H
#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "clock.h"
#include "IView.h"
#include "NavigationHandler.H"
#include "ControlContext.h"

class MainView : public IView{
  public:
    MainView(Clock& c, NavigationHandler& nav);
    void Render(int width, int height, uint8_t param);
    void Update();
    void GoToTimeSetting();
    void GoToAlarmsManager();
  private:
    void RefreshTime();
    ControlContext _controlContexts[2];
    Clock& refToC;
    NavigationHandler& refToNav;
    lv_obj_t* _clocklabel;
};
#endif