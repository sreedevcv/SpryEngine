#include <iostream>
#include <cmath>

#include "Entity.hpp"
#include "Mat4.hpp"
#include "Vec3.hpp"
#include "Transforms.hpp"

#include "Cuboid.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Tetrahedron.hpp"
#include "Camera.hpp"
#include "BasicMesh.hpp"

#include "utils.hpp"

class MyWindow : public spry::Window {
private:
    int m_width = 600;
    int m_height = 400;
    spry::Shader m_shader;
    spry::Tetrahedron cube;
    spry::Camera m_camera;
    spry::BasicMesh mesh;

protected:
    void updateFrame(float deltaTime)
    {
        auto model = spry::Mat4<float>(1.0f);
        auto view = m_camera.get_view_matrix();
        auto projection = m_camera.get_projection_matrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_shader.use();
        m_shader.setUniformMatrix("model", model);
        m_shader.setUniformMatrix("view", view);
        m_shader.setUniformMatrix("projection", projection);
        cube.draw();

        // mesh.draw();

        check_for_opengl_error();
    }

    void processInput(float deltaTime)
    {
        if (isPressed(GLFW_KEY_ESCAPE)) {
            closeWindow();
        }
        if (isPressed(GLFW_KEY_W)) {
            m_camera.process_movement(spry::Camera::movement::FORWARD, deltaTime);
        }
        if (isPressed(GLFW_KEY_S)) {
            m_camera.process_movement(spry::Camera::movement::BACKWARD, deltaTime);
        }
        if (isPressed(GLFW_KEY_A)) {
            m_camera.process_movement(spry::Camera::movement::LEFT, deltaTime);
        }
        if (isPressed(GLFW_KEY_D)) {
            m_camera.process_movement(spry::Camera::movement::RIGHT, deltaTime);
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

        float x_offset = x_pos - m_camera.mouse_data.last_x;
        float y_offset = m_camera.mouse_data.last_y - y_pos;

        m_camera.mouse_data.last_x = x_pos;
        m_camera.mouse_data.last_y = y_pos;

        m_camera.process_mouse_movement(x_offset, y_offset, true);
        // printMat(m_camera.get_view_matrix());
    }

    void on_mouse_scroll(double x_offset, double y_offset)
    {
        // std::cout << "Mouse scrolled\n";
        m_camera.process_mouse_scroll(static_cast<float>(y_offset));
    }

public:
    MyWindow(int width, int height)
        : Window(width, height, "Test")
        , m_width(width)
        , m_height(height)
        , m_shader("C:/Users/SHARIHA3/source/repos/SpryEngine/basic.vert", "C:/Users/SHARIHA3/source/repos/SpryEngine/basic.frag")
        , cube(m_shader)
        , m_camera(width, height)
    {
        glClearColor(0.8f, 0.9f, 0.85f, 1.0f);
        m_shader.compile();
        m_camera.mouse_data.first_mouse = true;
        capture_mouse();
        float a[] = {
            // positions        // Colors
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.1f, // front top left
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.1f, // front top right
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.1f, // front bottom left
        0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.1f, // front bottom right

        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.1f, // back top left
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.1f, // back top right
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.1f, // back bottom left
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.1f, // back bottom right
        };

        int b[] = {
        0, 3, 1, // front 1
        0, 2, 3, // front 2

        0, 4, 6, // left 1
        0, 6, 2, // left 2

        1, 7, 5, // right 1
        1, 3, 7, // right 2

        1, 4, 0, // top 1
        1, 5, 4, // top 2s

        4, 5, 7, // back 1
        5, 6, 7, // back 2

        3, 6, 2, // bottom 1
        3, 7, 6, // bottom 2
        };

        int c[] = {
            3, 4
        };

        mesh.load_data(&m_shader, std::span<float>{a}, std::span<int>{b}, std::span<int>{c});
    }
};

int main(int argc, char** argv)
{

    MyWindow w(800, 600);
    w.start();

    // spry::Vec3 a = spry::Vec3(1.0f, 0.0f, 0.0f);
    // spry::Vec3 b = spry::Vec3(0.0f, 1.0f, 0.0f);
    // auto c = a + b;

    // printVec(a);
    // printVec(b);
    // printVec(a + b);

    // std::cout << spry::radians(0.0f) << "\n";
    // std::cout << spry::radians(45.0f) << "\n";
    // std::cout << spry::radians(90.f) << "\n";
    // std::cout << spry::radians(270.0f) << "\n";
    // std::cout << spry::radians(180.0f) << "\n";
    // std::cout << spry::radians(360.0f) << "\n";

    // std::cout << std::sin(spry::radians(90.0f)) << "\n";

    return 0;
}