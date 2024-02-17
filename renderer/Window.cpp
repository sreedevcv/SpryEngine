#include "Window.hpp"

spry::Window::Window(int width, int height, const char *title)
    : mWidth(width), mHeight(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mWindow = glfwCreateWindow(mWidth, mHeight, title, nullptr, nullptr);

    if (mWindow == nullptr) 
    {
        std::cout << "Failed to load window\n";
        glfwTerminate();
        std::exit(-1);
    }

    const auto framebufferSizeCallback = [](GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    };

    glfwMakeContextCurrent(mWindow);
    glfwSetWindowUserPointer(mWindow, this);
    glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(-1);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "Window and OpenGL inititalized\n";
}

spry::Window::~Window()
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
    std::cout << "Window destroyed\n";
}

void spry::Window::start()
{
    double prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(mWindow))
    {
        double currTime = glfwGetTime();
        float deltaTime = static_cast<float>(currTime - prevTime);
        prevTime = currTime;

        processInput(deltaTime);
        updateFrame(deltaTime);

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
}

bool spry::Window::isPressed(int key)
{
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        return true;
    }
    return false;
}

void spry::Window::closeWindow()
{
    glfwSetWindowShouldClose(mWindow, true);
}
