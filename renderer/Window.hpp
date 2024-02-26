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
    virtual void process_input(float deltaTime) = 0;
    virtual void update_frame(float deltaTime) = 0;
    virtual void on_mouse_move(double x_pos_in, double y_pos_in) = 0;
    virtual void on_mouse_scroll(double x_offset, double y_offset) = 0;

public:
    Window(int width, int height, const char* title);
    virtual ~Window();

    bool is_key_pressed(int key);
    bool is_key_released(int key);
    double get_global_time();
    void capture_mouse(bool capture);
    void draw_wireframe(bool value);

    void start();
    void closeWindow();
};

}
