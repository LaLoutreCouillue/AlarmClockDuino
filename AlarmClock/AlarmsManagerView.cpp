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


AlarmsManagerView::AlarmsManagerView(AlarmsManager& am, NavigationHandler& nav) : refToAm(am), refToNav(nav) {}

void AlarmsManagerView::Render(int width, int height, uint8_t param) {
  //Clean old view
  lv_obj_clean(lv_scr_act());
  //Display & Grid Setup
  lv_obj_t* screen = lv_obj_create(lv_scr_act());
  lv_obj_set_size(screen, width, height);
  
  static lv_coord_t col_dsc[] = {
    LV_GRID_FR(2), LV_GRID_FR(2), LV_GRID_FR(1), LV_GRID_FR(1), 
    LV_GRID_TEMPLATE_LAST
  };

  static lv_coord_t row_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_TEMPLATE_LAST
  };

  lv_obj_t* grid = lv_obj_create(lv_scr_act());
  lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
  lv_obj_set_size(grid, width, height);

  //top row
  lv_obj_t* obj;
  lv_obj_t* label;
  lv_obj_t* control;
  const char* dayNames[7] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
  char result[50];
  //First button
  
  for(uint8_t i = 0; i < 3; i++) {  
    //Since each alarm contains two rows, we double i for accessing the current row.
    uint8_t row = i*2;
    result[0] = '\0';
    Alarm alarm = refToAm.GetAlarm(i);
    //Alarm Name
    obj = lv_obj_create(grid);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1,  //column
                      LV_GRID_ALIGN_STRETCH, row, 1);      //row
    label = lv_label_create(obj);
    lv_label_set_text_fmt(label, "Alarm %d", i);
    //Alarm Hour
    obj = lv_obj_create(grid);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 1, 1,  //column
                      LV_GRID_ALIGN_STRETCH, row, 1);      //row
    label = lv_label_create(obj);
    lv_label_set_text_fmt(label, "%02d:%02d", alarm.Hour, alarm.Minute);
    //Edit Button
    obj = lv_obj_create(grid);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 2, 1,  //column
                      LV_GRID_ALIGN_STRETCH, row, 1);      //row
    control = lv_btn_create(obj);
    lv_obj_set_size(control, LV_PCT(100), LV_PCT(100));
    lv_obj_center(control);
    lv_obj_add_event_cb(control, control_event, LV_EVENT_CLICKED, NULL);
    //Add the label
    label = lv_label_create(control);
    lv_label_set_text(label, "Edit");
    lv_obj_center(label);
    // Store the index and the instance as user data
    _controlContexts[row] = { this, 1, i };
    lv_obj_set_user_data(control, & _controlContexts[row]);
    //Active Days
    obj = lv_obj_create(grid);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 2,  //column
                      LV_GRID_ALIGN_STRETCH, row + 1, 1);      //row
    label = lv_label_create(obj);
    for(uint8_t j = 0; j < 7; j++) {  
      if (alarm.ActiveDay[j]) {
        if (strlen(result) > 0) {
          strcat(result, ", ");  // Add space if not the first entry
        } else {
          strcat(result, "Active Days : ");  // Add space if not the first entry
        }
        strcat(result, dayNames[j]);  // Append day name
      }
    }
    lv_label_set_text(label, result);
    
    //Switch
    obj = lv_obj_create(grid);
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 2, 1,  //column
                        LV_GRID_ALIGN_STRETCH, row+1, 1);      //row
    control = lv_switch_create(obj);
    lv_obj_set_size(control, LV_PCT(100), LV_PCT(100));
    lv_obj_center(control);
    if (alarm.IsActive) {
      lv_obj_add_state(control, LV_STATE_CHECKED);
    }else{
      lv_obj_remove_state(control, LV_STATE_CHECKED);
    }
    lv_obj_add_event_cb(control, control_event, LV_EVENT_VALUE_CHANGED, NULL);
    // Store the index and the instance as user data
    _controlContexts[row+1] = { this, 2, i };
    lv_obj_set_user_data(control, & _controlContexts[row+1]);
  }
  
  //Back button
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 3, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 0, 6);      //row
  _controlContexts[6] = { this, 3, 0 };
  AddButton(obj, btn_event, & _controlContexts[6], "Back");

}

void AlarmsManagerView::Update() {
}

void AlarmsManagerView::GoToMain(){
  refToNav.NavigateTo(e_Main, 0);
}

void AlarmsManagerView::ToggleAlarm(uint8_t id, bool value){
  refToAm.ToggleAlarm(id, value);
}

void AlarmsManagerView::EditAlarm(uint8_t id){
  refToNav.NavigateTo(e_AlarmEdition, id);
}
