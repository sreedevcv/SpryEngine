#include <iostream>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Cuboid.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Tetrahedron.hpp"
#include "Camera.hpp"
#include "PlaneMesh.hpp"
#include "Sphere.hpp"
#include "Line.hpp"

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
    spry::Sphere sphere;
    spry::Line x_axis;
    spry::Line y_axis;
    spry::Line z_axis;

protected:
    void update_frame(float delta_time) override
    {
        process_input(delta_time);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float angle = get_global_time();

        auto view = m_camera.get_view_matrix();
        auto projection = m_camera.get_projection_matrix();

        auto cube_model = glm::mat4(1.0f);
        cube_model = glm::rotate(cube_model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        cube_model = glm::translate(cube_model, glm::vec3(0.0f, sin(angle * 0.5f), 0.0f));
        cube_model = glm::scale(cube_model, glm::vec3(1.5f, 1.5f, 1.5f));

        m_vetex_and_color_shader.use();
        m_vetex_and_color_shader.set_uniform_matrix("model", cube_model);
        m_vetex_and_color_shader.set_uniform_matrix("view", view);
        m_vetex_and_color_shader.set_uniform_matrix("projection", projection);
        tetra.draw();

        auto plane_model = glm::mat4(1.0f);
        plane_model = glm::translate(plane_model, glm::vec3(-5.0f, -3.0f, -5.0f));
        plane_model = glm::rotate(plane_model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        m_vetex_shader.use();
        m_vetex_shader.set_uniform_matrix("model", plane_model);
        m_vetex_shader.set_uniform_matrix("view", view);
        m_vetex_shader.set_uniform_matrix("projection", projection);
        m_vetex_shader.set_uniform_vec4("color", glm::vec4(1.0f, 0.9f, 0.8f, 1.0f));
        plane.draw();

        auto sphere_model = glm::mat4(1.0f);
        sphere_model = glm::translate(sphere_model, glm::vec3(3.0f, 1.0f, -5.0f));
        m_vetex_shader.set_uniform_matrix("model", sphere_model);
        m_vetex_shader.set_uniform_vec4("color", glm::vec4(0.8f, 1.0f, 0.9f, 1.0f));
        sphere.draw();

        auto line_model = glm::mat4(1.0f);
        m_vetex_shader.set_uniform_matrix("model", line_model);
        m_vetex_shader.set_uniform_vec4("color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        x_axis.draw();
        m_vetex_shader.set_uniform_vec4("color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        y_axis.draw();
        m_vetex_shader.set_uniform_vec4("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        z_axis.draw();

        check_for_opengl_error();   
    }

    void process_input(float delta_time)
    {
        if (is_key_pressed(GLFW_KEY_ESCAPE)) {
            close_window();
        }
        if (is_key_pressed(GLFW_KEY_W)) {
            m_camera.process_movement(spry::Camera::movement::FORWARD, delta_time);
        }
        if (is_key_pressed(GLFW_KEY_S)) {
            m_camera.process_movement(spry::Camera::movement::BACKWARD, delta_time);
        }
        if (is_key_pressed(GLFW_KEY_A)) {
            m_camera.process_movement(spry::Camera::movement::LEFT, delta_time);
        }
        if (is_key_pressed(GLFW_KEY_D)) {
            m_camera.process_movement(spry::Camera::movement::RIGHT, delta_time);
        }
    }

    void on_mouse_move(double x_pos_in, double y_pos_in) override
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

    void on_mouse_scroll(double x_offset, double y_offset) override
    {
        m_camera.process_mouse_scroll(static_cast<float>(y_offset));
    }

    void on_screen_size_change(int width, int height) override
    {
        m_camera.set_screen_size(width, height);
    }

public:
    MyWindow(int width, int height)
        : Window(width, height, "Test")
        , m_width(width)
        , m_height(height)
        , m_vetex_and_color_shader("../test/basic.vert", "../test/basic.frag")
        , m_vetex_shader("../test/planeMesh.vert", "../test/planeMesh.frag")
    {
        set_mouse_capture(true);
        // set_wireframe_mode(true);

        plane.load(10.0f, 10.0f, 3, 3);
        sphere.load(2.0f, 20, 20);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        m_camera.set_screen_size(width, height);
        m_camera.mouse_data.first_mouse = true;
        m_camera.m_position = glm::vec3(0.0f, 1.0f, 3.0f);
        m_vetex_and_color_shader.compile();
        m_vetex_shader.compile();

        x_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
        y_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
        z_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f));
        check_for_opengl_error();
    }
};

int main(int argc, char** argv)
{
    MyWindow w(800, 600);
    w.start();
    return 0;
}

/* ray tracer
 * lines
 * points
 * spheres
 */