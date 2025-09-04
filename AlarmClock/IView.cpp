#include "iView.h"


lv_obj_t* IView::CreateGridAndInitScreen(int width, int height, lv_coord_t col_dsc[], lv_coord_t row_dsc[]){
  //Display & Grid Setup
  lv_obj_t* screen = lv_obj_create(lv_scr_act());
  lv_obj_set_size(screen, width, height);
  lv_obj_t* grid = lv_obj_create(lv_scr_act());
  lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
  lv_obj_set_size(grid, width, height);

  return grid;
}


lv_obj_t* IView:: CreateCell(lv_obj_t* grid, uint8_t column, uint8_t columnSpan, uint8_t row, uint8_t rowSpan){
  lv_obj_t* cell = lv_obj_create(grid);
  lv_obj_set_grid_cell(cell, LV_GRID_ALIGN_STRETCH, column, columnSpan,  //column
                      LV_GRID_ALIGN_STRETCH, row, rowSpan);      //row
  return cell;
}


void IView::AddButton(lv_obj_t* parent, lv_event_cb_t callback, ControlContext* controlContext, const char* text, uint8_t flex){
  //Create the button
  lv_obj_t* button = lv_btn_create(parent);
  if (flex == 0){
    lv_obj_set_size(button, LV_PCT(100), LV_PCT(100));
  }else{
    lv_obj_set_flex_grow(button, flex); 
  }
  lv_obj_center(button);
  //Add the function to the button
  lv_obj_add_event_cb(button, callback, LV_EVENT_CLICKED, NULL);
  // Store the controlContext in the button to get it back after
  lv_obj_set_user_data(button, controlContext);
  //Add the label
  lv_obj_t* label = lv_label_create(button);
  lv_label_set_text(label, text);
  lv_obj_center(label);
}

void IView::AddSwitch(lv_obj_t* parent, lv_event_cb_t callback, ControlContext* controlContext, bool checked, uint8_t flex){
  //Create the switch
  lv_obj_t* switchs = lv_switch_create(parent);
  if (flex == 0){
    lv_obj_set_size(switchs, LV_PCT(100), LV_PCT(100));
  }else{
    lv_obj_set_flex_grow(switchs, flex); 
  }
  lv_obj_center(switchs);
  //Add the function to the switch
  lv_obj_add_event_cb(switchs, callback, LV_EVENT_VALUE_CHANGED, NULL);
  // Store the controlContext in the switch to get it back after
  lv_obj_set_user_data(switchs, controlContext);
  //Check if needed
  if (checked) {
    lv_obj_add_state(switchs, LV_STATE_CHECKED);
  }else{
    lv_obj_remove_state(switchs, LV_STATE_CHECKED);
  }
}

void IView::AddCheckbox(lv_obj_t* parent, lv_event_cb_t callback, ControlContext* controlContext, bool checked, const char* text, uint8_t flex){
  //Create the checkbox
  lv_obj_t* checkbox = lv_checkbox_create(parent);
  if (flex == 0){
    lv_obj_set_size(checkbox, LV_PCT(100), LV_PCT(100));
  }else{
    lv_obj_set_flex_grow(checkbox, flex); 
  }
  //Add the function to the checkbox
  lv_obj_add_event_cb(checkbox, callback, LV_EVENT_VALUE_CHANGED, NULL);
  // Store the controlContext in the switch to get it back after
  lv_obj_set_user_data(checkbox, controlContext);
  //Check if needed
  if (checked) {
    lv_obj_add_state(checkbox, LV_STATE_CHECKED);
  }else{
    lv_obj_remove_state(checkbox, LV_STATE_CHECKED);
  }
  //Set text
  lv_checkbox_set_text(checkbox, text);
}
