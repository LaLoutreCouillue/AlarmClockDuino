#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "MainView.h"
#include "Arduino.h"


static void btn_event(lv_event_t * e) {
  //The original target of the event. Can be the buttons or the container
  lv_obj_t * btn = (lv_obj_t*)lv_event_get_target(e);

  //Get Back the structure
  ControlContext* ctx = (ControlContext*)lv_obj_get_user_data(btn);
  MainView* instance = static_cast<MainView*>(ctx->instance); //store the instance
  uint8_t action = ctx->action;

  switch(action){
      case 1: //Time Setting
        instance->GoToTimeSetting();
        break;
      case 2: //Alarm Setting
        instance->GoToAlarmsManager();
        break;
  }
}


MainView::MainView(Clock& c, NavigationHandler& nav) : refToC(c), refToNav(nav) {}

void MainView::Render(int width, int height, uint8_t param) {
  //Clean old view
  lv_obj_clean(lv_scr_act());
  //Display & Grid Setup
  lv_obj_t* screen = lv_obj_create(lv_scr_act());
  lv_obj_set_size(screen, width, height);
  
  static lv_coord_t col_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), 
    LV_GRID_TEMPLATE_LAST
  };

  static lv_coord_t row_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_TEMPLATE_LAST
  };

  lv_obj_t* grid = lv_obj_create(lv_scr_act());
  lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
  lv_obj_set_size(grid, width, height);

  //top row
  lv_obj_t* obj;
  lv_obj_t* label;
  //First button
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 0, 1);      //row
  _controlContexts[0] = { this, 1, 0 };
  AddButton(obj, btn_event, & _controlContexts[0], "TimeSettings");
  //Second button
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 1, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 0, 1);      //row
  _controlContexts[1] = { this, 2, 0 };
  AddButton(obj, btn_event, & _controlContexts[1], "Alarm Settings");

  //middle row
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 2,  //column
                      LV_GRID_ALIGN_STRETCH, 1, 1);      //row
  _clocklabel = lv_label_create(obj);
  RefreshTime();

  //bottom row
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 2,  //column
                      LV_GRID_ALIGN_STRETCH, 2, 1);      //row
}

void MainView::Update() {
  RefreshTime();
}

void MainView::RefreshTime(){
  lv_label_set_text(_clocklabel, refToC.GetTimeAsString().c_str());
}

void MainView::GoToTimeSetting(){
  refToNav.NavigateTo(e_TimeSetting, 0);
}

void MainView::GoToAlarmsManager(){
  refToNav.NavigateTo(e_AlarmsManager, 0);
}