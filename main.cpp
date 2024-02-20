#include <iostream>

#include "Entity.hpp"
#include "Mat4.hpp"
#include "Vec2.hpp"

#include "Cuboid.hpp"
#include "Shader.hpp"
#include "Window.hpp"

class MyWindow : public spry::Window {
private:
    int mWidth = 600;
    int mHeight = 400;
    spry::Shader mShader;
    spry::Cuboid cube;

protected:
    void updateFrame(float deltaTime)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mShader.use();
        cube.draw();
    }

    void processInput(float deltaTime)
    {
        if (isPressed(GLFW_KEY_ESCAPE)) {
            closeWindow();
        }
    }

public:
    MyWindow()
        : Window(600, 400, "Test")
        , mShader("basic.vert", "basic.frag")
        , cube(mShader)
    {
        glClearColor(0.2f, 0.1f, 0.23f, 1.0f);
    }
};

int main(int argc, char** argv)
{
    std::cout << "Hello, world!" << std::endl;

    MyWindow w;
    w.start();

    return 0;
}