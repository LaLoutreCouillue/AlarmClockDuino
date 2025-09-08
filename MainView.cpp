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


MainView::MainView(Clock& c, NavigationHandler& nav) : _refToC(c), _refToNav(nav) {}

void MainView::Render(int width, int height, uint8_t param) {
  //Clean old view
  CleanScreen();

  static lv_coord_t col_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), 
    LV_GRID_TEMPLATE_LAST
  };
  static lv_coord_t row_dsc[] = {
    LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1),
    LV_GRID_TEMPLATE_LAST
  };
  lv_obj_t* grid = CreateGridAndInitScreen(width, height, col_dsc, row_dsc);
  
  lv_obj_t* cell;
  //top row
  //First button
  cell = CreateCell(grid, 0, 1, 0, 1);
  _controlContexts[0] = { this, 1, 0 };
  CreateButton(cell, btn_event, & _controlContexts[0], "TimeSettings");
  //Second button
  cell = CreateCell(grid, 1, 1, 0, 1);
  _controlContexts[1] = { this, 2, 0 };
  CreateButton(cell, btn_event, & _controlContexts[1], "Alarm Settings");

  //middle row
  cell = CreateCell(grid, 0, 2, 1, 1);
  _clocklabel = CreateLabel(cell, 48, "");
  RefreshTime();

  //bottom row
  cell = CreateCell(grid, 0, 2, 2, 1);
}

void MainView::Update() {
  RefreshTime();
}

void MainView::RefreshTime(){
  lv_label_set_text(_clocklabel, _refToC.GetTimeAsString().c_str());
}

void MainView::GoToTimeSetting(){
  _refToNav.NavigateTo(e_TimeSetting, 0);
}

void MainView::GoToAlarmsManager(){
  _refToNav.NavigateTo(e_AlarmsManager, 0);
}