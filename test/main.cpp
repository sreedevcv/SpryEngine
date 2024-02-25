#include <iostream>

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
    spry::Tetrahedron cube;
    spry::Camera m_camera;
    spry::PlaneMesh plane;

protected:
    void updateFrame(float deltaTime)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto glm_model = glm::mat4(1.0f);
        auto glm_view = m_camera.get_view_matrix();
        auto glm_projection = m_camera.get_projection_matrix();

        m_vetex_and_color_shader.use();
        m_vetex_and_color_shader.set_uniform_matrix("model", glm_model);
        m_vetex_and_color_shader.set_uniform_matrix("view", glm_view);
        m_vetex_and_color_shader.set_uniform_matrix("projection", glm_projection);
        cube.draw();

        m_vetex_shader.use();
        m_vetex_shader.set_uniform_matrix("model", glm_model);
        m_vetex_shader.set_uniform_matrix("view", glm_view);
        m_vetex_shader.set_uniform_matrix("projection", glm_projection);
        // plane.draw();

        // const auto prt = [](glm::mat4& m) {
        //     for (int i = 0; i < 4; i++) {
        //         for (int j = 0; j < 4; j++) {
        //             std::cout << m[i][j] << " ";
        //         }
        //         std::cout << "\n";
        //     }
        //     std::cout << "-------\n";
        // };

        // printMat(view);
        // prt(glm_view);

        // printMat(projection);
        // prt(glm_projection);

        check_for_opengl_error();
    }

    void processInput(float deltaTime)
    {
        if (is_pressed(GLFW_KEY_ESCAPE)) {
            closeWindow();
        }
        if (is_pressed(GLFW_KEY_W)) {
            m_camera.process_movement(spry::Camera::movement::FORWARD, deltaTime);
        }
        if (is_pressed(GLFW_KEY_S)) {
            m_camera.process_movement(spry::Camera::movement::BACKWARD, deltaTime);
        }
        if (is_pressed(GLFW_KEY_A)) {
            m_camera.process_movement(spry::Camera::movement::LEFT, deltaTime);
        }
        if (is_pressed(GLFW_KEY_D)) {
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
        , m_camera(width, height)
    {
        glClearColor(0.8f, 0.9f, 0.85f, 1.0f);
        capture_mouse();

        m_vetex_and_color_shader.compile();
        m_vetex_shader.compile();
        plane.load(10.0f, 10.0f, 4, 4);
        // m_camera.m_position = spry::Vec3(0.0f, 10.0f, 10.0f);

        m_camera.mouse_data.first_mouse = true;
    }
};

int main(int argc, char** argv)
{
    MyWindow w(800, 600);
    w.start();
    return 0;
}