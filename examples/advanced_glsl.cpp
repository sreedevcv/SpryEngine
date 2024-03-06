#include <iostream>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include "Cuboid.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "Line.hpp"
#include "Transform.hpp"
#include "PlaneMesh.hpp"
#include "Sphere.hpp"
#include "Point.hpp"

#include "utils.hpp"
#include "CubeMap.hpp"

class MyWindow : public spry::Window {
private:
    int m_width = 600;
    int m_height = 400;

    spry::Shader shader = spry::ShaderManager::mvp_shader();
    spry::Camera m_camera;
    spry::Line x_axis;
    spry::Line y_axis;
    spry::Line z_axis;
    spry::Cuboid cube;
    spry::Sphere sphere;
    spry::Shader point_shader;
    spry::Point points;

protected:
    void update_frame(float delta_time) override
    {
        process_input(delta_time);

        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // draw_points();

        auto model = glm::mat4(1.0f);
        auto view = m_camera.get_view_matrix();
        auto projection = m_camera.get_projection_matrix();

        shader.use();
        shader.set_uniform_matrix("projection", projection);
        shader.set_uniform_matrix("view", view);
        draw_axes();

        check_for_opengl_error();

        point_shader.use();
        check_for_opengl_error();
        point_shader.set_uniform_matrix("model", spry::Transform().get_model());
        point_shader.set_uniform_matrix("view", view);
        point_shader.set_uniform_matrix("projection", projection);
        points.draw();

        check_for_opengl_error();

        // close_window();
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
        if (is_key_pressed(GLFW_KEY_SPACE)) {
            m_camera.process_movement(spry::Camera::movement::UP, delta_time);
        }
        if (is_key_pressed(GLFW_KEY_LEFT_SHIFT)) {
            m_camera.process_movement(spry::Camera::movement::DOWN, delta_time);
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

    void draw_axes()
    {
        auto line_model = glm::mat4(1.0f);
        shader.set_uniform_matrix("model", line_model);
        shader.set_uniform_vec("color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        x_axis.draw();
        shader.set_uniform_vec("color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        y_axis.draw();
        shader.set_uniform_vec("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        z_axis.draw();
    }

    void draw_points()
    {
        auto model = glm::mat4(1.0f);
        auto view = m_camera.get_view_matrix();
        auto projection = m_camera.get_projection_matrix();

        shader.use();
        shader.set_uniform_matrix("projection", projection);
        shader.set_uniform_matrix("view", view);
        draw_axes();

        check_for_opengl_error();

        point_shader.use();
        check_for_opengl_error();
        point_shader.set_uniform_matrix("model", spry::Transform().get_model());
        point_shader.set_uniform_matrix("view", view);
        point_shader.set_uniform_matrix("projection", projection);
        points.draw();
    }

public:
    MyWindow(int width, int height)
        : Window(width, height, "Rendering To Texture")
        , m_width(width)
        , m_height(height)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        set_mouse_capture(true);

        m_camera.set_screen_size(width, height);
        m_camera.mouse_data.first_mouse = true;
        m_camera.m_position = glm::vec3(0.0f, 1.0f, 3.0f);

        x_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
        y_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
        z_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f));

        sphere.load(1.0f, 20, 20);

        std::vector<glm::vec3> point_locs = {
            glm::vec3(1.0f, 1.0f, 0.0f),
            glm::vec3(-1.0f, -1.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
        };
        points.load(point_locs);

        point_shader.set_shader_code(
            R"(
                #version 330 core
                layout (location = 0) in vec3 position;

                uniform mat4 model;
                uniform mat4 view;
                uniform mat4 projection;

                void main() {
                    gl_Position = projection * view * model * vec4(position, 1.0);
                    gl_PointSize = gl_Position.z;
                }
            )",
            R"(
                #version 330 core
                out vec4 frag_color;

                void main() {
                    frag_color = vec4(0.8, 0.9, 0.7, 1.0);
                }
            )");
        check_for_opengl_error();
    }
};

int main(int argc, char** argv)
{
    // stbi_set_flip_vertically_on_load(true);

    MyWindow w(800, 600);
    w.start();

    return 0;
}

/* ray tracer
 * fogs
 * sky box
 * text
 */
