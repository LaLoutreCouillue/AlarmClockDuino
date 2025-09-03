#ifndef IVIEW_H
#define IVIEW_H

class IView {
public:
    virtual void Update() = 0;  // Called every loop cycle to update logic
    virtual void Render(int width, int height, uint8_t param) = 0;  // Called to draw or print the UI
    virtual ~IView() {}         // Virtual destructor (important!)
};

#endif