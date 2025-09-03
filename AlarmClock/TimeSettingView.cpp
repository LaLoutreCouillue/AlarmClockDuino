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

TimeSettingView::TimeSettingView(Clock& c, NavigationHandler& nav) : refToC(c), refToNav(nav) {}

void TimeSettingView::Render(int width, int height, uint8_t param) {
  //Clean old view
  lv_obj_clean(lv_scr_act());
  //Display & Grid Setup
  lv_obj_t* screen = lv_obj_create(lv_scr_act());
  lv_obj_set_size(screen, width, height);

  static lv_coord_t col_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(2),
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_TEMPLATE_LAST
  };

  static lv_coord_t row_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_TEMPLATE_LAST
  };
  
  lv_obj_t* grid = lv_obj_create(lv_scr_act());
  lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
  lv_obj_set_size(grid, width, height);

  //variables
  lv_obj_t* obj;
  lv_obj_t* label;
  lv_obj_t* button;
  _time = refToC.GetTime();

  //top row
  //create top row + buttons
  for(uint8_t i = 0; i < 5; i++) {    
    obj = lv_obj_create(grid);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, i, 1,  //column
                        LV_GRID_ALIGN_STRETCH, 0, 1);      //row
    
    //Create the button
    button = lv_btn_create(obj);
    lv_obj_set_size(button, LV_PCT(100), LV_PCT(100));
    lv_obj_center(button);
    lv_obj_add_event_cb(button, btn_event, LV_EVENT_CLICKED, NULL);
    //Add the label
    label = lv_label_create(button);
    lv_label_set_text(label, "+");
    lv_obj_center(label);

    // Store the index and the instance as user data
    _controlContexts[i] = { this, 1, i };
    lv_obj_set_user_data(button, &_controlContexts[i]);
  }
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 5, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 0, 1);      //row
  
  //Create the button
  button = lv_btn_create(obj);
  lv_obj_set_size(button, LV_PCT(100), LV_PCT(100));
  lv_obj_center(button);
  lv_obj_add_event_cb(button, btn_event, LV_EVENT_CLICKED, NULL);
  //Add the label
  label = lv_label_create(button);
  lv_label_set_text(label, "Save");
  lv_obj_center(label);
  // Store the index and the instance as user data
  _controlContexts[5] = { this, 3, 0 };
  lv_obj_set_user_data(button, &_controlContexts[5]);

  //middle row
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 1, 1);      //row
  _dayLabel = lv_label_create(obj);
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 1, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 1, 1);      //row
  _monthLabel = lv_label_create(obj);
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 2, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 1, 1);      //row
  _yearLabel = lv_label_create(obj);
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 3, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 1, 1);      //row
  _hourLabel = lv_label_create(obj);
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 4, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 1, 1);      //row
  _minuteLabel = lv_label_create(obj);
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 5, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 1, 1);      //row
  RefreshTime();
  //bottom row
  //create bottom row - buttons
  for(uint8_t i = 0; i < 5; i++) {    
    obj = lv_obj_create(grid);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, i, 1,  //column
                        LV_GRID_ALIGN_STRETCH, 2, 1);      //row
    
    //Create the button
    button = lv_btn_create(obj);
    lv_obj_set_size(button, LV_PCT(100), LV_PCT(100));
    lv_obj_center(button);
    lv_obj_add_event_cb(button, btn_event, LV_EVENT_CLICKED, NULL);
    //Add the label
    label = lv_label_create(button);
    lv_label_set_text(label, "-");
    lv_obj_center(label);

    // Store the index and the instance as user data
    _controlContexts[i + 6] = { this, 2, i };
    lv_obj_set_user_data(button, &_controlContexts[i + 6]);
  }
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 5, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 2, 1);      //row
  
  //Create the button
  button = lv_btn_create(obj);
  lv_obj_set_size(button, LV_PCT(100), LV_PCT(100));
  lv_obj_center(button);
  lv_obj_add_event_cb(button, btn_event, LV_EVENT_CLICKED, NULL);
  //Add the label
  label = lv_label_create(button);
  lv_label_set_text(label, "Exit");
  lv_obj_center(label);
  // Store the index and the instance as user data
    _controlContexts[11] = { this, 4, 0 };
  lv_obj_set_user_data(button, & _controlContexts[11]);
}

void TimeSettingView::Update() {
  
}

void TimeSettingView::SaveTime(){
  refToC.SetTime(_time);
}

void TimeSettingView::GoToMain(){
  refToNav.NavigateTo(e_Main, 0);
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
