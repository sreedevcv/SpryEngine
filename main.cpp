#include <iostream>
#include <glad/glad.h>

#include "Entity.hpp"
#include "Vec2.hpp"
#include "Mat4.hpp"

#include "Window.hpp"

class MyWindow: public spry::Window 
{
private:
    int mWidth  = 600;
    int mHeight = 400;

protected:
    void updateFrame(float deltaTime) 
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void processInput(float deltaTime)
    {
        if (isPressed(GLFW_KEY_ESCAPE))
        {
            closeWindow();
        }
    }

public:
    MyWindow(): Window(600, 400, "Test")
    {
        glClearColor(0.2f, 0.1f, 0.23f, 1.0f);
    }

};

int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;

    MyWindow w;
    w.start();

    return 0;
}