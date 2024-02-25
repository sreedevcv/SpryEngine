#include "Window.hpp"

spry::Window::Window(int width, int height, const char* title)
    : m_width(width)
    , m_height(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);

    if (m_window == nullptr) {
        std::cout << "Failed to load window\n";
        glfwTerminate();
        std::exit(-1);
    }

    const auto framebufferSizeCallback = [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    };

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    static const auto mouse_move_callback = [](GLFWwindow *glfw_window, double x_pos_in, double y_pos_in) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        window->on_mouse_move(x_pos_in, y_pos_in);
    };

    static const auto mouse_scroll_callback = [](GLFWwindow *glfw_window, double x_offset, double y_offset) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        window->on_mouse_scroll(x_offset, y_offset);
    };

    glfwSetCursorPosCallback(m_window, mouse_move_callback);
    glfwSetScrollCallback(m_window, mouse_scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
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
    glfwDestroyWindow(m_window);
    glfwTerminate();
    std::cout << "Window destroyed\n";
}

void spry::Window::start()
{
    double prevTime = glfwGetTime();

    while (!glfwWindowShouldClose(m_window)) {
        double currTime = glfwGetTime();
        float deltaTime = static_cast<float>(currTime - prevTime);
        prevTime = currTime;

        processInput(deltaTime);
        updateFrame(deltaTime);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

bool spry::Window::is_pressed(int key)
{
    if (glfwGetKey(m_window, key) == GLFW_PRESS) {
        return true;
    }
    return false;
}

void spry::Window::capture_mouse()
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void spry::Window::release_mouse()
{
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

double spry::Window::get_global_time()
{
    return glfwGetTime();
}

void spry::Window::closeWindow()
{
    glfwSetWindowShouldClose(m_window, true);
}
