#ifndef MAINVIEW_H
#define MAINVIEW_H
#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "clock.h"
#include "IView.h"
#include "NavigationHandler.H"
#include "ButtonContext.h"

class MainView : public IView{
  public:
    MainView(Clock& c, NavigationHandler& nav);
    void Render(int width, int height);
    void Update();
    void GoToTimeSetting();
  private:
    ButtonContext _buttonContexts[2];
    Clock& refToC;
    NavigationHandler& refToNav;
    lv_obj_t* _clocklabel;
};
#endif