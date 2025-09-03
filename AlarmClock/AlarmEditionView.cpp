#include "widgets/label/lv_label.h"
#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "AlarmEditionView.h"
#include "Arduino.h"


static void btn_event(lv_event_t * e) {
  //The original target of the event. Can be the buttons or the container
  lv_obj_t * btn = (lv_obj_t*)lv_event_get_target(e);

  //Get Back the structure
  ControlContext* ctx = (ControlContext*)lv_obj_get_user_data(btn);
  AlarmEditionView* instance = static_cast<AlarmEditionView*>(ctx->instance);; //store the instance
  uint8_t id = ctx->id; //store the index
  uint8_t action = ctx->action;

  switch(action){
      case 1: //Increase
        instance->IncreaseTime(id);
        break;
      case 2: //Decrease
        instance->DecreaseTime(id);
        break;
      case 3: //Toggle
        instance->ToggleDay(id, lv_obj_has_state(btn, LV_STATE_CHECKED));
        break;
      case 4: //SaveAlarm
        instance->SaveAlarm();
        break;
      case 5: //Go to main page
        instance->GoBack();
        break;
  }
}

AlarmEditionView::AlarmEditionView(AlarmsManager& am, NavigationHandler& nav) : refToAm(am), refToNav(nav) {}

void AlarmEditionView::Render(int width, int height, uint8_t param) {
  //Clean old view
  lv_obj_clean(lv_scr_act());
  //Display & Grid Setup
  lv_obj_t* screen = lv_obj_create(lv_scr_act());
  lv_obj_set_size(screen, width, height);

  static lv_coord_t col_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(2),
    LV_GRID_FR(1), 
    LV_GRID_TEMPLATE_LAST
  };

  static lv_coord_t row_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_TEMPLATE_LAST
  };
  
  lv_obj_t* grid = lv_obj_create(lv_scr_act());
  lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
  lv_obj_set_size(grid, width, height);

  //variables
  lv_obj_t* obj;
  lv_obj_t* label;
  lv_obj_t* button;
  _alarmId = param;
  _alarm = refToAm.GetAlarm(_alarmId);

  //Row 1
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 3,  //column
                      LV_GRID_ALIGN_STRETCH, 0, 1);      //row
  label = lv_label_create(obj);
  lv_label_set_text(label, "Alarm");
  
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 3, 1,  //column
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
  _controlContexts[0] = { this, 4, 0 };
  lv_obj_set_user_data(button, &_controlContexts[0]);

  //Row 2
  //create + buttons
  for(uint8_t i = 0; i < 2; i++) {    
    obj = lv_obj_create(grid);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, i, 1,  //column
                        LV_GRID_ALIGN_STRETCH, 1, 1);      //row
    
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
    _controlContexts[i + 1] = { this, 1, i };
    lv_obj_set_user_data(button, &_controlContexts[i + 1]);
  }

  //Row 3
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 2, 1);      //row
  _hourLabel = lv_label_create(obj);
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 1, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 2, 1);      //row
  _minuteLabel = lv_label_create(obj);
  RefreshTime();

  //Row 4
  //create - buttons
  for(uint8_t i = 0; i < 2; i++) {    
    obj = lv_obj_create(grid);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, i, 1,  //column
                        LV_GRID_ALIGN_STRETCH, 3, 1);      //row
    
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
    _controlContexts[i + 3] = { this, 2, i };
    lv_obj_set_user_data(button, &_controlContexts[i + 3]);
  }

  //Multi Row
  //create checkboxes
  const char* dayNames[7] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
  obj = lv_obj_create(grid);  
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 2, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 1, 3);      //row
  lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_COLUMN);
  for(uint8_t i = 0; i < 7; i++) {
    button = lv_checkbox_create(obj);
    lv_checkbox_set_text(button, dayNames[i]);
    if (_alarm.ActiveDay[i]) {
      lv_obj_add_state(button, LV_STATE_CHECKED);
    }else{
      lv_obj_remove_state(button, LV_STATE_CHECKED);
    }
    lv_obj_add_event_cb(button, btn_event, LV_EVENT_VALUE_CHANGED, NULL);
    // Store the index and the instance as user data
    _controlContexts[i+5] = { this, 3, i };
    lv_obj_set_user_data(button, & _controlContexts[i+5]);
  }

  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 3, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 3, 1);      //row
  
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
    _controlContexts[12] = { this, 5, 0 };
  lv_obj_set_user_data(button, & _controlContexts[12]);
}

void AlarmEditionView::Update() {
  
}

void AlarmEditionView::SaveAlarm(){
  refToAm.ModifyAlarm(_alarmId, _alarm);
}

void AlarmEditionView::GoBack(){
  refToNav.NavigateTo(e_AlarmsManager, 0);
}

void AlarmEditionView::DecreaseTime(uint8_t id){
  switch(id) {
      case 0: //Hour
          if (_alarm.Hour > 1){
            _alarm.Hour -= 1;
          } else {
            _alarm.Hour = 23;
          }
          break;
      case 1: //Minutes
          if (_alarm.Minute > 1){
            _alarm.Minute -= 1;
          } else {
            _alarm.Minute = 59;
          }
          break;
      // etc.
  }
  RefreshTime();
}

void AlarmEditionView::IncreaseTime(uint8_t id){
  switch(id) {
      case 0: //Hour
          if (_alarm.Hour < 23){
            _alarm.Hour += 1;
          } else {
            _alarm.Hour = 0;
          }
          break;
      case 1: //Minutes
          if (_alarm.Minute < 59){
            _alarm.Minute += 1;
          } else {
            _alarm.Minute = 0;
          }
          break;
      // etc.
  }
  RefreshTime();
}

void AlarmEditionView::ToggleDay(uint8_t id, bool value){
  _alarm.ActiveDay[id] = value;
}

void AlarmEditionView::RefreshTime(){
  lv_label_set_text_fmt(_hourLabel, "%02d", _alarm.Hour);
  lv_label_set_text_fmt(_minuteLabel, "%02d", _alarm.Minute);
}
