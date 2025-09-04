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

AlarmEditionView::AlarmEditionView(AlarmsManager& am, NavigationHandler& nav) : _refToAm(am), _refToNav(nav) {}

void AlarmEditionView::Render(int width, int height, uint8_t param) {
  //Clean old view
  lv_obj_clean(lv_scr_act());

  static lv_coord_t col_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(2),
    LV_GRID_FR(1), 
    LV_GRID_TEMPLATE_LAST
  };
  static lv_coord_t row_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_TEMPLATE_LAST
  };
  lv_obj_t* grid = CreateGridAndInitScreen(width, height, col_dsc, row_dsc);

  //variables
  lv_obj_t* cell;
  lv_obj_t* label;
  _alarmId = param;
  _alarm = _refToAm.GetAlarm(_alarmId);

  //Row 1
  cell = CreateCell(grid, 0, 3, 0, 1);
  label = lv_label_create(cell);
  lv_label_set_text(label, "Alarm");
  
  cell = CreateCell(grid, 3, 1, 0, 1);
  _controlContexts[0] = { this, 4, 0 };
  AddButton(cell, btn_event, & _controlContexts[0], "Save");

  //Row 2
  //create + buttons
  for(uint8_t i = 0; i < 2; i++) {    
    cell = CreateCell(grid, i, 1, 1, 1);
    _controlContexts[i + 1] = { this, 1, i };
    AddButton(cell, btn_event, & _controlContexts[i + 1], "+");
  }

  //Row 3
  cell = CreateCell(grid, 0, 1, 2, 1);
  _hourLabel = lv_label_create(cell);
  cell = CreateCell(grid, 1, 1, 2, 1);
  _minuteLabel = lv_label_create(cell);
  RefreshTime();

  //Row 4
  //create - buttons
  for(uint8_t i = 0; i < 2; i++) {    
    cell = CreateCell(grid, i, 1, 3, 1);
    _controlContexts[i + 3] = { this, 2, i };
    AddButton(cell, btn_event, & _controlContexts[i + 3], "-");
  }

  //Multi Row
  //create checkboxes
  cell = CreateCell(grid, 2, 1, 1, 3);
  lv_obj_set_flex_flow(cell, LV_FLEX_FLOW_COLUMN);
  for(uint8_t i = 0; i < 7; i++) {
    _controlContexts[i+5] = { this, 3, i };
    AddCheckbox(cell, btn_event, &_controlContexts[i+5], _alarm.ActiveDay[i], _dayNames[i], 1);
  }

  cell = CreateCell(grid, 3, 1, 3, 1);
  _controlContexts[12] = { this, 5, 0 };
  AddButton(cell, btn_event, & _controlContexts[12], "Exit");
}

void AlarmEditionView::Update() {
  
}

void AlarmEditionView::SaveAlarm(){
  _refToAm.ModifyAlarm(_alarmId, _alarm);
}

void AlarmEditionView::GoBack(){
  _refToNav.NavigateTo(e_AlarmsManager, 0);
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
