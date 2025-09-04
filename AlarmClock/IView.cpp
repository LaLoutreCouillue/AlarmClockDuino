#include "iView.h"

void IView::AddButton(lv_obj_t* parent, lv_event_cb_t callback, ControlContext* controlContext, const char* text){
  //Create the button
  lv_obj_t* button = lv_btn_create(parent);
  lv_obj_set_size(button, LV_PCT(100), LV_PCT(100));
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
