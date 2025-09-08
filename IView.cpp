#include "iView.h"

void IView::CleanScreen(){
  lv_obj_clean(lv_scr_act());
}

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

lv_obj_t* IView::CreateLabel(lv_obj_t* parent, uint8_t fontSize, const char* fmt, ...) {

  lv_obj_t* label = lv_label_create(parent);

  // Setup font if provided
  switch (fontSize){
    case 16 :
      lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
    break;
    case 24 :
      lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    break;
    case 32:
      lv_obj_set_style_text_font(label, &lv_font_montserrat_32, 0);
    break;
    case 40:
      lv_obj_set_style_text_font(label, &lv_font_montserrat_40, 0);
    break;
    case 48:
      lv_obj_set_style_text_font(label, &lv_font_montserrat_48, 0);
    break;
  }
  lv_obj_center(label);

  // Handle optional formatting
  va_list args;
  va_start(args, fmt);

  if (strchr(fmt, '%')) {
    char buf[128];
    vsnprintf(buf, sizeof(buf), fmt, args);
    lv_label_set_text(label, buf);  // Uses variadic args
  } else {
    lv_label_set_text(label, fmt);
  }

  
  va_end(args);

  return label;
}

lv_obj_t* IView::CreateButton(lv_obj_t* parent, lv_event_cb_t callback, ControlContext* controlContext, const char* text, uint8_t fontSize){
  //Create the button
  lv_obj_t* button = lv_btn_create(parent);
  lv_obj_set_size(button, LV_PCT(100), LV_PCT(100));
  lv_obj_center(button);
  //Add the function to the button
  lv_obj_add_event_cb(button, callback, LV_EVENT_CLICKED, NULL);
  // Store the controlContext in the button to get it back after
  lv_obj_set_user_data(button, controlContext);
  //Add the label
  CreateLabel(button, fontSize, text);
  return button;
}

lv_obj_t* IView::CreateSwitch(lv_obj_t* parent, lv_event_cb_t callback, ControlContext* controlContext, bool checked){
  //Create the switch
  lv_obj_t* switchs = lv_switch_create(parent);
  lv_obj_set_size(switchs, LV_PCT(100), LV_PCT(100));
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
  return switchs;
}

lv_obj_t* IView::CreateCheckbox(lv_obj_t* parent, lv_event_cb_t callback, ControlContext* controlContext, bool checked, const char* text){
  //Create the checkbox
  lv_obj_t* checkbox = lv_checkbox_create(parent);
  lv_obj_set_size(checkbox, LV_PCT(100), LV_PCT(100));
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
  return checkbox;
}
