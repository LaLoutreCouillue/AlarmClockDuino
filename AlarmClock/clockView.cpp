#include "Arduino_H7_Video.h"
#include "lvgl.h"
#include "Arduino_GigaDisplayTouch.h"
#include "ClockView.h"
#include "Arduino.h"


lv_obj_t* label;

ClockView::ClockView(Clock& c, NavigationHandler& nav) : refToC(c), refToNav(nav) {}

void ClockView::Render(int width, int height) {
  delay(3000);
  //Display & Grid Setup
  lv_obj_t* screen = lv_obj_create(lv_scr_act());
  lv_obj_set_size(screen, width, height);

  static lv_coord_t col_dsc[] = { 800, LV_GRID_TEMPLATE_LAST };
  static lv_coord_t row_dsc[] = { 160, 160, 160, LV_GRID_TEMPLATE_LAST };

  lv_obj_t* grid = lv_obj_create(lv_scr_act());
  lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
  lv_obj_set_size(grid, width, height);

  //top row
  lv_obj_t* obj;
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 0, 1);      //row

  //middle row
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 1, 1);      //row
  label = lv_label_create(obj);

  //bottom row
  obj = lv_obj_create(grid);
  lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, 0, 1,  //column
                      LV_GRID_ALIGN_STRETCH, 2, 1);      //row
}

void ClockView::Update() {
  lv_label_set_text(label, refToC.GetTime().c_str());
  lv_timer_handler();
}