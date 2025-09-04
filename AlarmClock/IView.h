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
    static lv_obj_t* CreateGridAndInitScreen(int width, int height, lv_coord_t col_dsc[], lv_coord_t row_dsc[]);
    static lv_obj_t* CreateCell(lv_obj_t* grid, uint8_t column, uint8_t columnSpan, uint8_t row, uint8_t rowSpan);
    static void AddButton(lv_obj_t* parent, lv_event_cb_t callback, ControlContext* controlContext, const char* text, uint8_t flex = 0);
    static void AddSwitch(lv_obj_t* parent, lv_event_cb_t callback, ControlContext* controlContext, bool checked, uint8_t flex = 0);
    static void AddCheckbox(lv_obj_t* parent, lv_event_cb_t callback, ControlContext* controlContext, bool checked, const char* text, uint8_t flex = 0);
    static inline const char* _dayNames[7] = {
        "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
    };
};

#endif