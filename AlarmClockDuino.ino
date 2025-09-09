#include "Clock.h"
#include "MainView.h"
#include "TimeSettingView.h"
#include "AlarmsManagerView.h"
#include "AlarmEditionView.h"
#include "AlarmsManager.h"
#include "NavigationHandler.h"
#include <Arduino_H7_Video.h>
#include <lvgl.h>
#include "Arduino_GigaDisplayTouch.h"

Arduino_H7_Video Display(800, 480, GigaDisplayShield);
Arduino_GigaDisplayTouch TouchDetector;

unsigned long lastUpdate = 0;

Clock _clock;
AlarmsManager _alarmsManager;
NavigationHandler _navigationHandler;
MainView _mainView(_clock, _navigationHandler);
TimeSettingView _timeSettingView(_clock, _navigationHandler);
AlarmsManagerView _alarmsManagerView(_alarmsManager, _navigationHandler);
AlarmEditionView _alarmEditionView(_alarmsManager, _navigationHandler);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.write("Begin");
  Display.begin();
  TouchDetector.begin();
  _clock.Init();
  _navigationHandler.Init();
  _alarmsManager.Init();
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long now = millis();
  
  // Call LVGL timer handler frequently
  lv_timer_handler();

  //Show Requested View
  switch (_navigationHandler.GetRequestedView()) {
    case e_None:
      // statements
      break;
    case e_Main:
      _mainView.Render(Display.width(), Display.height(), _navigationHandler.GetParam());
      _navigationHandler.NavigationDone();
      break;
    case e_TimeSetting:
      _timeSettingView.Render(Display.width(), Display.height(), _navigationHandler.GetParam());
      _navigationHandler.NavigationDone();
      break;
    case e_AlarmsManager:
      _alarmsManagerView.Render(Display.width(), Display.height(), _navigationHandler.GetParam());
      _navigationHandler.NavigationDone();
      break;
    case e_AlarmEdition:
      _alarmEditionView.Render(Display.width(), Display.height(), _navigationHandler.GetParam());
      _navigationHandler.NavigationDone();
      break;
    default:
      // statements
      break;
  }
  //Update Views
  
  // Update views every second
  if (now - lastUpdate >= 1000) {
    lastUpdate = now;
    switch (_navigationHandler.GetCurrentView()) {
        case e_None:
          // statements
          break;
        case e_Main:
          _mainView.Update();
          break;
        case e_TimeSetting:
          _timeSettingView.Update();
          break;
        case e_AlarmsManager:
          _alarmsManagerView.Update();
          break;
        case e_AlarmEdition:
          _alarmEditionView.Update();
          break;
        default:
          // statements
          break;
      }
  }

  delay(5);  // Small delay to avoid CPU overload

}
