#ifndef NAVIGATIONHANDLER_H
#define NAVIGATIONHANDLER_H
#include "Views.h"

class NavigationHandler {
  public:
    void Init();
    void NavigateTo(e_Views requestedView);
    void NavigationDone();
    e_Views GetRequestedView();
    e_Views GetCurrentView();
  private:
    e_Views CurrentView = e_None;
    e_Views RequestedView = e_None;
};

#endif