#ifndef ALARMSMANAGERVIEW_H
#define ALARMSMANAGERVIEW_H
#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "AlarmsManager.h"
#include "IView.h"
#include "NavigationHandler.H"
#include "ControlContext.h"

class AlarmsManagerView : public IView{
  public:
    AlarmsManagerView(AlarmsManager& am, NavigationHandler& nav);
    void Render(int width, int height, uint8_t param);
    void Update();
    void GoToMain();
    void EditAlarm(uint8_t id);
    void ToggleAlarm(uint8_t id, bool value);
  private:
    ControlContext _controlContexts[7];
    AlarmsManager& _refToAm;
    NavigationHandler& _refToNav;
};
#endif