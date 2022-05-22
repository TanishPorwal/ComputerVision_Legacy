#pragma once

#include "Image.h"

class Window
{
public:
    Window() = default;

    void setWindowName(const std::string& windowName);
    void showWindow(Image& img);
    void closeWindow(Image& img); // Just to close window and not remove it from memory as if want to see again, can save the time of allocating
    void destroyWindow(); // destroys the window / removes from memory

private:
    std::string m_WindowName;
};

