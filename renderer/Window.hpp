#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace spry
{
    class Window
    {
    private:
        int mWidth;
        int mHeight;

        GLFWwindow *mWindow;
    
    protected:
        virtual void processInput(float deltaTime) = 0;
        virtual void updateFrame(float deltaTime) = 0;

    public:
        Window(int width, int height, const char* title);
        virtual ~Window();

        void start();
        bool isPressed(int key);

        void closeWindow();
    };
    
} // namespace spry
