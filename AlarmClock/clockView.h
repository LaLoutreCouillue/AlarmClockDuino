#ifndef CLOCKSVIEW_H
#define CLOCKSVIEW_H
#include "clock.h"
#include "IView.h"
#include "NavigationHandler.H"

class ClockView : public IView{
  public:
    ClockView(Clock& c, NavigationHandler& nav);
    void Render(int width, int height);
    void Update();
private:
    Clock& refToC;
    NavigationHandler& refToNav;
};
#endif