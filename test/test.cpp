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