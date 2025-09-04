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
    void AddButton(lv_obj_t* parent, lv_event_cb_t callback, const char* text, ControlContext* controlContext);
};

#endif