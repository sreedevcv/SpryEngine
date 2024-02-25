#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace spry {
class Window {
private:
    int m_width;
    int m_height;

    GLFWwindow* m_window;

protected:
    virtual void processInput(float deltaTime) = 0;
    virtual void updateFrame(float deltaTime) = 0;
    virtual void on_mouse_move(double x_pos_in, double y_pos_in) = 0;
    virtual void on_mouse_scroll(double x_offset, double y_offset) = 0;

public:
    Window(int width, int height, const char* title);
    virtual ~Window();

    bool is_pressed(int key);
    void capture_mouse();
    void release_mouse();
    double get_global_time();


    void start();
    void closeWindow();
};

}
