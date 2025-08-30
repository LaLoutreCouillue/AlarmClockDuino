#include "Clock.h"
#include "ClockView.h"
#include "AlarmsManager.h"
#include "NavigationHandler.h"
#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "Arduino_GigaDisplayTouch.h"

Arduino_H7_Video Display(800, 480, GigaDisplayShield);
Arduino_GigaDisplayTouch TouchDetector;

unsigned long lastUpdate = 0;

Clock _clock;
AlarmsManager _alarmsManager;
NavigationHandler _navigationHandler;
ClockView _clockView(_clock, _navigationHandler);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.write("Begin");
  Display.begin();
  TouchDetector.begin();
  _clock.Init();
  _navigationHandler.Init();
  _alarmsManager.Init();
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
    case e_Clock:
      // statements
      _clockView.Render(Display.width(), Display.height());
      _navigationHandler.NavigationDone();
      break;
    case e_Settings:
      // statements
      break;
    case e_Alarm:
      // statements
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
        case e_Clock:
          _clockView.Update();
          break;
        case e_Settings:
          // statements
          break;
        case e_Alarm:
          // statements
          break;
        default:
          // statements
          break;
      }
  }

  delay(5);  // Small delay to avoid CPU overload

}
