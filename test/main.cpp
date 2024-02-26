/*
#include <iostream>

#include <chrono>
#include <thread>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Cuboid.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Tetrahedron.hpp"
#include "Camera.hpp"
#include "BasicMesh.hpp"
#include "PlaneMesh.hpp"
#include "utils.hpp"

class MyWindow : public spry::Window {
private:
    int m_width = 600;
    int m_height = 400;

    spry::Shader m_vetex_and_color_shader;
    spry::Shader m_vetex_shader;
    spry::Camera m_camera;
    spry::Tetrahedron tetra;
    spry::Cuboid cube;
    spry::PlaneMesh plane;

protected:
    void update_frame(float deltaTime)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float radius = 1.0f;
        // float camX = sin(get_global_time()) * radius;
        // float camZ = cos(get_global_time()) * radius;
        // auto view = glm::lookAt(glm::vec3(camX, camX, camZ), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // m_camera.m_yaw += camX * 0.01f;
        // m_camera.m_pitch += camX * 0.01f;
        // m_camera.update_camera_vectors();
        auto view = m_camera.get_view_matrix();
        auto projection = m_camera.get_projection_matrix();

        auto cube_model = glm::mat4(1.0f);
        m_vetex_and_color_shader.use();
        m_vetex_and_color_shader.set_uniform_matrix("model", cube_model);
        m_vetex_and_color_shader.set_uniform_matrix("view", view);
        m_vetex_and_color_shader.set_uniform_matrix("projection", projection);
        cube.draw();

        auto plane_model = glm::mat4(1.0f);
        plane_model = glm::translate(plane_model, glm::vec3(-5.0f, 0.0f, -5.0f));
        plane_model = glm::rotate(plane_model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        m_vetex_shader.use();
        m_vetex_shader.set_uniform_matrix("model", plane_model);
        m_vetex_shader.set_uniform_matrix("view", view);
        m_vetex_shader.set_uniform_matrix("projection", projection);
        plane.draw();

        check_for_opengl_error();

        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void process_input(float deltaTime)
    {
        if (is_key_pressed(GLFW_KEY_ESCAPE)) {
            closeWindow();
        }
        if (is_key_pressed(GLFW_KEY_W)) {
            m_camera.process_movement(spry::Camera::movement::FORWARD, deltaTime);
        }
        if (is_key_pressed(GLFW_KEY_S)) {
            m_camera.process_movement(spry::Camera::movement::BACKWARD, deltaTime);
        }
        if (is_key_pressed(GLFW_KEY_A)) {
            m_camera.process_movement(spry::Camera::movement::RIGHT, deltaTime);
        }
        if (is_key_pressed(GLFW_KEY_D)) {
            m_camera.process_movement(spry::Camera::movement::LEFT, deltaTime);
        }
    }

    void on_mouse_move(double x_pos_in, double y_pos_in)
    {
        float x_pos = static_cast<float>(x_pos_in);
        float y_pos = static_cast<float>(y_pos_in);

        if (m_camera.mouse_data.first_mouse) {
            m_camera.mouse_data.last_x = x_pos;
            m_camera.mouse_data.last_y = y_pos;
            m_camera.mouse_data.first_mouse = false;
        }

        // std::cout << m_camera.m_up.x << " " << m_camera.m_up.y << " " << m_camera.m_up.z << "\n";
        // std::cout << x_pos_in << " " << y_pos_in << "\n";

        float x_offset = x_pos - m_camera.mouse_data.last_x;
        float y_offset = m_camera.mouse_data.last_y - y_pos;

        m_camera.mouse_data.last_x = x_pos;
        m_camera.mouse_data.last_y = y_pos;

        m_camera.process_mouse_movement(x_offset, y_offset, true);
    }

    void on_mouse_scroll(double x_offset, double y_offset)
    {
        m_camera.process_mouse_scroll(static_cast<float>(y_offset));
    }

public:
    MyWindow(int width, int height)
        : Window(width, height, "Test")
        , m_width(width)
        , m_height(height)
        , m_vetex_and_color_shader("../test/basic.vert", "../test/basic.frag")
        , m_vetex_shader("../test/planeMesh.vert", "../test/planeMesh.frag")
    {
        capture_mouse(true);
        draw_wireframe(true);

        plane.load(10.0f, 10.0f, 3, 3);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        m_camera.set_screen_size(width, height);
        m_camera.mouse_data.first_mouse = true;
        // m_camera.m_position = glm::vec3(0.0f, 5.0f, 10.0f);
        m_vetex_and_color_shader.compile();
        m_vetex_shader.compile();
    }
};

int main(int argc, char** argv)
{
    MyWindow w(800, 600);
    w.start();
    return 0;
}
*/


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "Tetrahedron.hpp"

#include <iostream>
#include <cstdlib>
#include <cmath>

int screenWidth = 800;
int screenHeight = 600;
GLFWwindow* window = nullptr;
double prevTicks;
double lastMouseX, lastMouseY;

////////////////////////
/// system callbacks
////////////////////////
void sys_mouseCB(GLFWwindow* window, double x, double y);

////////////////////////
/// glfw callback
////////////////////////
int initGLFW();
void initGL();

void initMem();
void destroyMem();
void reshapeCB(GLFWwindow* window, int w, int h);
void keyboardCB(double deltaTime);
void mouseCB(GLFWwindow* window, double dx, double dy);
void mouseScrollCB(GLFWwindow* window, double dx, double dy);
void update(double dt);
void render();

////////////////////////
/// global variables
////////////////////////
GLuint vao;
GLuint vbo;
spry::Shader shader("../test/basic.vert", "../test/basic.frag");
GLuint containerTexture;
GLuint awesomeTexture;

glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
float camYaw = -90.0f, camPitch = 0.0f;
float camFov = 45.0f;

////////////////////////
// implementations
////////////////////////
int initGLFW()
{
    std::cout << "hello" << std::endl;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(screenWidth, screenHeight, "argv[0]", nullptr, nullptr);
    if (window == nullptr) {
        // lgl::error::ErrorWarn("Failed to create GLFW window");
        std::exit(1);
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, reshapeCB);
    glfwSetCursorPosCallback(window, sys_mouseCB);
    glfwSetScrollCallback(window, mouseScrollCB);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        // lgl::error::ErrorWarn("Failed to initalize GLAD");
        std::exit(1);
        return -1;
    }

    glViewport(0, 0, screenWidth, screenHeight);

    return 0;
}

void initGL()
{
}

void update(double dt)
{
}

glm::mat4 selfImplemented_lookAt(glm::vec3 pos, glm::vec3 targetPos, glm::vec3 up);

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    shader.use();

    glm::mat4 view = selfImplemented_lookAt(camPos, camPos + camFront, camUp);
    shader.set_uniform_matrix("view", view);
    glm::mat4 projection = glm::perspective(glm::radians(camFov), screenWidth * 1.0f / screenHeight, 0.1f, 100.0f);
    shader.set_uniform_matrix("projection", projection);
    glm::mat4 model = glm::mat4(1.0f);
    const float angle = 20.0f;
    
    // model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    shader.set_uniform_matrix("model", model);
}

void reshapeCB(GLFWwindow* window, int w, int h)
{
    screenWidth = w;
    screenHeight = h;

    glViewport(0, 0, static_cast<GLsizei>(screenWidth), static_cast<GLsizei>(screenHeight));
}

void sys_mouseCB(GLFWwindow* window, double x, double y)
{
    static bool firstMouse = true;
    if (firstMouse) {
        lastMouseX = x;
        lastMouseY = y;
        firstMouse = false;
    }

    float xoffset = x - lastMouseX;
    float yoffset = lastMouseY - y;
    lastMouseX = x;
    lastMouseY = y;

    static float kSensitivity = 0.05f;
    xoffset *= kSensitivity;
    yoffset *= kSensitivity;

    mouseCB(window, xoffset, yoffset);
}

void mouseCB(GLFWwindow* window, double x, double y)
{
    camYaw = std::fmod(camYaw + x, 360.0f);
    camPitch += y;

    if (camPitch > 89.0f)
        camPitch = 89.0f;
    if (camPitch < -89.0f)
        camPitch = -89.0f;

    glm::vec3 front = glm::vec3(
        std::cos(glm::radians(camYaw)) * std::cos(glm::radians(camPitch)),
        std::sin(glm::radians(camPitch)),
        std::sin(glm::radians(camYaw)) * std::cos(glm::radians(camPitch)));
    camFront = glm::normalize(front);
}

void mouseScrollCB(GLFWwindow* window, double dx, double dy)
{
    if (camFov >= 1.0f && camFov <= 45.0f)
        camFov -= dy;
    if (camFov < 1.0f)
        camFov = 1.0f;
    if (camFov > 45.0f)
        camFov = 45.0f;
}

void keyboardCB(double deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        return;
    }

    const float kCamSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camPos += kCamSpeed * camFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camPos -= kCamSpeed * camFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camPos += kCamSpeed * glm::normalize(glm::cross(camUp, camFront));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camPos += kCamSpeed * glm::normalize(glm::cross(camFront, camUp));
}

void initMem()
{
}

void destroyMem()
{
}

glm::mat4 selfImplemented_lookAt(glm::vec3 pos, glm::vec3 targetPos, glm::vec3 up)
{
    // inverse because the scene will be affected, not actually camera
    glm::vec3 front = glm::normalize(pos - targetPos);
    glm::vec3 right = glm::normalize(glm::cross(up, front));
    glm::vec3 up_ = glm::cross(front, right);

    glm::mat4 translation = glm::mat4(1.0f);
    translation[3][0] = -pos.x;
    translation[3][1] = -pos.y;
    translation[3][2] = -pos.z;

    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = right.x;
    rotation[0][1] = up_.x;
    rotation[0][2] = front.x;

    rotation[1][0] = right.y;
    rotation[1][1] = up_.y;
    rotation[1][2] = front.y;

    rotation[2][0] = right.z;
    rotation[2][1] = up_.z;
    rotation[2][2] = front.z;

    return rotation * translation;
}

int main()
{
    initGLFW();
    initGL();

    spry::Tetrahedron tetra;
    shader.compile();

    std::cout << "hai\n" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        double delta = glfwGetTime() - prevTicks;
        prevTicks = glfwGetTime();

        keyboardCB(delta);
        update(delta);
        render();
        tetra.draw();

        glfwSwapBuffers(window);
    }

    destroyMem();
    glfwTerminate();

    return 0;
}