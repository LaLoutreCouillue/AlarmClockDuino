#include "widgets/label/lv_label.h"
#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "TimeSettingView.h"
#include "Arduino.h"


static void btn_event(lv_event_t * e) {
  //The original target of the event. Can be the buttons or the container
  lv_obj_t * btn = (lv_obj_t*)lv_event_get_target(e);

  //Get Back the structure
  ControlContext* ctx = (ControlContext*)lv_obj_get_user_data(btn);
  TimeSettingView* instance = static_cast<TimeSettingView*>(ctx->instance);; //store the instance
  uint8_t id = ctx->id; //store the index
  uint8_t action = ctx->action;

  switch(action){
      case 1: //Increase
        instance->IncreaseTime(id);
        break;
      case 2: //Decrease
        instance->DecreaseTime(id);
        break;
      case 3: //SaveTime
        instance->SaveTime();
        break;
      case 4: //Go to main page
        instance->GoToMain();
        break;
  }
}

TimeSettingView::TimeSettingView(Clock& c, NavigationHandler& nav) : _refToC(c), _refToNav(nav) {}

void TimeSettingView::Render(int width, int height, uint8_t param) {
  //Clean old view
  CleanScreen();

  static lv_coord_t col_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(2),
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_TEMPLATE_LAST
  };
  static lv_coord_t row_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_TEMPLATE_LAST
  };
  lv_obj_t* grid = CreateGridAndInitScreen(width, height, col_dsc, row_dsc);

  //variables
  lv_obj_t* cell;
  _time = _refToC.GetTime();

  //top row
  //create top row + buttons
  for(uint8_t i = 0; i < 5; i++) {    
    cell = CreateCell(grid, i, 1, 0, 1);
    _controlContexts[i] = { this, 1, i };
    CreateButton(cell, btn_event, & _controlContexts[i], "+");
  }
  cell = CreateCell(grid, 5, 1, 0, 1);
  _controlContexts[5] = { this, 3, 0 };
  CreateButton(cell, btn_event, & _controlContexts[5], "Save");

  //middle row
  cell = CreateCell(grid, 0, 1, 1, 1);
  _dayLabel = CreateLabel(cell, 48, "");
  cell = CreateCell(grid, 1, 1, 1, 1);
  _monthLabel = CreateLabel(cell, 48, "");
  cell = CreateCell(grid, 2, 1, 1, 1);
  _yearLabel = CreateLabel(cell, 48, "");
  cell = CreateCell(grid, 3, 1, 1, 1);
  _hourLabel = CreateLabel(cell, 48, "");
  cell = CreateCell(grid, 4, 1, 1, 1);
  _minuteLabel = CreateLabel(cell, 48, "");
  cell = CreateCell(grid, 5, 1, 1, 1);
  RefreshTime();
  //bottom row
  //create bottom row - buttons
  for(uint8_t i = 0; i < 5; i++) {    
    cell = CreateCell(grid, i, 1, 2, 1);
    _controlContexts[i + 6] = { this, 2, i };
    CreateButton(cell, btn_event, & _controlContexts[i + 6], "-");
  }
  cell = CreateCell(grid, 5, 1, 2, 1);
  _controlContexts[11] = { this, 4, 0 };
  CreateButton(cell, btn_event, & _controlContexts[11], "Exit");
}

void TimeSettingView::Update() {
  
}

void TimeSettingView::SaveTime(){
  _refToC.SetTime(_time);
}

void TimeSettingView::GoToMain(){
  _refToNav.NavigateTo(e_Main, 0);
}

void TimeSettingView::DecreaseTime(uint8_t id){
  switch(id) {
      case 0: //Day
          if (_time.tm_mday > 2){
            _time.tm_mday -= 1;
          } else {
            _time.tm_mday = 31;
          }
          break;
      case 1: //Month
          if (_time.tm_mon > 1){
            _time.tm_mon -= 1;
          } else {
            _time.tm_mon = 11;
          }
          break;
      case 2: //Year
          _time.tm_year -= 1;
          break;
      case 3: //Hour
          if (_time.tm_hour > 1){
            _time.tm_hour -= 1;
          } else {
            _time.tm_hour = 23;
          }
          break;
      case 4: //Minutes
          if (_time.tm_min > 1){
            _time.tm_min -= 1;
          } else {
            _time.tm_min = 59;
          }
          break;
      // etc.
  }
  RefreshTime();
}

void TimeSettingView::IncreaseTime(uint8_t id){
  switch(id) {
      case 0: //Day
          if (_time.tm_mday < 31){
            _time.tm_mday += 1;
          } else {
            _time.tm_mday = 1;
          }
          break;
      case 1: //Month
          if (_time.tm_mon < 11){
            _time.tm_mon += 1;
          } else {
            _time.tm_mon = 0;
          }
          break;
      case 2: //Year
          _time.tm_year += 1;
          break;
      case 3: //Hour
          if (_time.tm_hour < 23){
            _time.tm_hour += 1;
          } else {
            _time.tm_hour = 0;
          }
          break;
      case 4: //Minutes
          if (_time.tm_min < 59){
            _time.tm_min += 1;
          } else {
            _time.tm_min = 0;
          }
          break;
      // etc.
  }
  RefreshTime();
}

void TimeSettingView::RefreshTime(){
  lv_label_set_text_fmt(_dayLabel, "%d", _time.tm_mday);
  lv_label_set_text_fmt(_monthLabel, "%d", _time.tm_mon + 1);
  lv_label_set_text_fmt(_yearLabel, "%d", _time.tm_year + 1900);
  lv_label_set_text_fmt(_hourLabel, "%02d", _time.tm_hour);
  lv_label_set_text_fmt(_minuteLabel, "%02d", _time.tm_min);
}
