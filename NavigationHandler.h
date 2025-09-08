#ifndef NAVIGATIONHANDLER_H
#define NAVIGATIONHANDLER_H
#include "Views.h"
#include "Arduino.h"

class NavigationHandler {
  public:
    void Init();
    void NavigateTo(e_Views requestedView, uint8_t param);
    void NavigationDone();
    e_Views GetRequestedView();
    e_Views GetCurrentView();
    uint8_t GetParam();
  private:
    e_Views _currentView = e_None;
    e_Views _requestedView = e_None;
    uint8_t _param = 0;
};

#endif