#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "AlarmsManagerView.h"
#include "Arduino.h"


static void control_event(lv_event_t * e) {
  //The original target of the event. Can be the buttons or the container
  lv_obj_t * control = (lv_obj_t*)lv_event_get_target(e);

  //Get Back the structure
  ControlContext* ctx = (ControlContext*)lv_obj_get_user_data(control);
  AlarmsManagerView* instance = static_cast<AlarmsManagerView*>(ctx->instance); //store the instance
  uint8_t action = ctx->action;
  uint8_t id = ctx->id;


  switch(action){
      case 1: //Edit
        instance->EditAlarm(id);
        break;
      case 2: //Toggle
        instance->ToggleAlarm(id, lv_obj_has_state(control, LV_STATE_CHECKED));
        break;
      case 3: //Done
        instance->GoToMain();
        break;
  }
}


AlarmsManagerView::AlarmsManagerView(AlarmsManager& am, NavigationHandler& nav) : _refToAm(am), _refToNav(nav) {}

void AlarmsManagerView::Render(int width, int height, uint8_t param) {
  //Clean old view
  CleanScreen();
  
  static lv_coord_t col_dsc[] = {
    LV_GRID_FR(2), LV_GRID_FR(2), LV_GRID_FR(1), LV_GRID_FR(1), 
    LV_GRID_TEMPLATE_LAST
  };
  static lv_coord_t row_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_TEMPLATE_LAST
  };
  lv_obj_t* grid = CreateGridAndInitScreen(width, height, col_dsc, row_dsc);

  //top row
  lv_obj_t* cell;
  lv_obj_t* label;
  char result[50];
  //First button
  
  for(uint8_t i = 0; i < 3; i++) {  
    //Since each alarm contains two rows, we double i for accessing the current row.
    uint8_t row = i*2;
    result[0] = '\0';
    Alarm alarm = _refToAm.GetAlarm(i);
    //Alarm Name
    cell = CreateCell(grid, 0, 1, row, 1);
    CreateLabel(cell, 40, "Alarm %d", i);
    //Alarm Hour
    cell = CreateCell(grid, 1, 1, row, 1);
    CreateLabel(cell, 40, "%02d:%02d", alarm.Hour, alarm.Minute);
    //Edit Button
    cell = CreateCell(grid, 2, 1, row, 1);
    _controlContexts[row] = { this, 1, i };
    CreateButton(cell, control_event, & _controlContexts[row], "Edit");
    //Active Days
    cell = CreateCell(grid, 0, 2, row + 1, 1);
    for(uint8_t j = 0; j < 7; j++) {  
      if (alarm.ActiveDay[j]) {
        if (strlen(result) > 0) {
          strcat(result, ", ");  // Add space if not the first entry
        } else {
          strcat(result, "Active Days : ");  // Add space if not the first entry
        }
        strcat(result, _dayNames[j]);  // Append day name
      }
    }
    CreateLabel(cell, 32, result);
    
    //Switch
    cell = CreateCell(grid, 2, 1, row+1, 1);
    _controlContexts[row+1] = { this, 2, i };
    CreateSwitch(cell, control_event, &_controlContexts[row+1], alarm.IsActive);
  }
  
  //Back button
  cell = CreateCell(grid, 3, 1, 0, 6);
  _controlContexts[6] = { this, 3, 0 };
  CreateButton(cell, control_event, & _controlContexts[6], "Back");

}

void AlarmsManagerView::Update() {
}

void AlarmsManagerView::GoToMain(){
  _refToNav.NavigateTo(e_Main, 0);
}

void AlarmsManagerView::ToggleAlarm(uint8_t id, bool value){
  _refToAm.ToggleAlarm(id, value);
}

void AlarmsManagerView::EditAlarm(uint8_t id){
  _refToNav.NavigateTo(e_AlarmEdition, id);
}
