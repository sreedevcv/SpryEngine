#include <iostream>

#include "Window.hpp"

class GameWindow: public spry::Window {
private:
    int m_width;
    int m_height;

    void update_frame(float delta_time) override
    {

    }

public:
    GameWindow(int width, int height)
        : Window(width, height, "Rubiks Cube")
        , m_width(width)
        , m_height(height)
    {
        glClearColor(0.9, 0.8, 0.7, 1.0);
    }
};

int main(int argc, char** argv)
{
    GameWindow w(800, 600);
    w.start();
    return 0;
}
