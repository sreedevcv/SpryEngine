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
    virtual void update_frame(float deltaTime) = 0;
    virtual void on_mouse_move(double x_pos_in, double y_pos_in);
    virtual void on_mouse_scroll(double x_offset, double y_offset);
    virtual void on_screen_size_change(int width, int height);

public:
    Window(int width, int height, const char* title);
    virtual ~Window();

    bool is_key_pressed(int key);
    bool is_key_released(int key);
    double get_global_time();
    void set_mouse_capture(bool capture);
    void set_wireframe_mode(bool value);

    void start();
    void close_window();
};

}
