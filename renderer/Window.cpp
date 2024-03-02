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

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    static const auto mouse_move_callback = [](GLFWwindow* glfw_window, double x_pos_in, double y_pos_in) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        window->on_mouse_move(x_pos_in, y_pos_in);
    };

    static const auto mouse_scroll_callback = [](GLFWwindow* glfw_window, double x_offset, double y_offset) {
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        window->on_mouse_scroll(x_offset, y_offset);
    };

    const auto framebuffer_size_callback = [](GLFWwindow* glfw_window, int width, int height) {
        glViewport(0, 0, width, height);
        auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
        window->on_screen_size_change(width, height);
    };

    glfwSetCursorPosCallback(m_window, mouse_move_callback);
    glfwSetScrollCallback(m_window, mouse_scroll_callback);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        std::exit(-1);
    }

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

        update_frame(deltaTime);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

bool spry::Window::is_key_pressed(int key)
{
    if (glfwGetKey(m_window, key) == GLFW_PRESS) {
        return true;
    }
    return false;
}

// Does not work
bool spry::Window::is_key_released(int key)
{
    if (glfwGetKey(m_window, key) == GLFW_RELEASE) {
        return false;
    }
    return true;
}

void spry::Window::set_mouse_capture(bool capture)
{
    if (capture) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

double spry::Window::get_global_time()
{
    return glfwGetTime();
}

void spry::Window::set_wireframe_mode(bool value)
{
    if (value) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void spry::Window::close_window()
{
    glfwSetWindowShouldClose(m_window, true);
}

void spry::Window::on_mouse_move(double x_pos_in, double y_pos_in)
{
}

void spry::Window::on_mouse_scroll(double x_offset, double y_offset)
{
}

void spry::Window::on_screen_size_change(int width, int height)
{
}
