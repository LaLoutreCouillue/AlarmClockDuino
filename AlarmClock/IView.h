#ifndef IVIEW_H
#define IVIEW_H
#include "Arduino.h"
#include "lvgl.h"
#include "ControlContext.h"

class IView {
public:
    virtual void Update() = 0;  // Called every loop cycle to update logic
    virtual void Render(int width, int height, uint8_t param) = 0;  // Called to draw or print the UI
    virtual ~IView() {}         // Virtual destructor (important!)
protected:
    static void CleanScreen();
    static lv_obj_t* CreateGridAndInitScreen(int width, int height, lv_coord_t col_dsc[], lv_coord_t row_dsc[]);
    static lv_obj_t* CreateCell(lv_obj_t* grid, uint8_t column, uint8_t columnSpan, uint8_t row, uint8_t rowSpan);
    static lv_obj_t* CreateLabel(lv_obj_t* parent, uint8_t fontSize, const char* fmt, ...);
    static lv_obj_t* CreateButton(lv_obj_t* parent, lv_event_cb_t callback, ControlContext* controlContext, const char* text, uint8_t fontSize = 24);
    static lv_obj_t* CreateSwitch(lv_obj_t* parent, lv_event_cb_t callback, ControlContext* controlContext, bool checked);
    static lv_obj_t* CreateCheckbox(lv_obj_t* parent, lv_event_cb_t callback, ControlContext* controlContext, bool checked, const char* text);
    static inline const char* _dayNames[7] = {
        "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
    };
};

#endif