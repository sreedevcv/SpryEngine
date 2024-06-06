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
#include "Quad.hpp"
#include "Texture.hpp"

#include "utils.hpp"


class BlendingWindow : public spry::Window {
private:
    int m_width = 600;
    int m_height = 400;

    spry::Shader shader = spry::ShaderManager::mvp_shader();
    spry::Shader grass_quad_shader;
    spry::Camera m_camera;
    spry::Line x_axis;
    spry::Line y_axis;
    spry::Line z_axis;
    spry::Cuboid cube;
    spry::PlaneMesh plane;
    spry::Quad quad;
    spry::Transform plane_transform;
    spry::Texture grass_texture;

    std::vector<glm::vec3> vegetation;

protected:
    void update_frame(float delta_time) override
    {
        process_input(delta_time);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto model = glm::mat4(1.0f);
        auto view = m_camera.get_view_matrix();
        auto projection = m_camera.get_projection_matrix();

        shader.use();
        shader.set_uniform_matrix("projection", projection);
        shader.set_uniform_matrix("view", view);
        draw_axes();

        // shader.set_uniform_vec("color", glm::vec4(0.9f, 0.7f, 0.9f, 1.0f));
        // cube.draw();
        // shader.set_uniform_matrix("model", spry::Transform().rotate(glm::radians(get_global_time() * 40), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f))).get_model());

        shader.set_uniform_vec("color", glm::vec4(0.5f, 0.4f, 0.4f, 1.0f));
        shader.set_uniform_matrix("model", plane_transform.get_model());
        plane.draw();

        grass_quad_shader.use();
        grass_quad_shader.set_uniform_matrix("view", view);
        grass_quad_shader.set_uniform_matrix("projection", projection);

        grass_texture.bind();
        for (auto loc : vegetation) {
            loc.y += 0.5;
            float angle = get_signed_angle(glm::vec3(0.0f, 0.0f, 1.0f), glm::normalize(m_camera.m_position), glm::vec3(0.0f, 1.0f, 0.0f));

            auto grass_quad_model = spry::Transform()
                                        .rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f))
                                        .translate(loc)
                                        .scale(glm::vec3(0.5f, 0.5f, 0.5f))
                                        .get_model();
            grass_quad_shader.set_uniform_matrix("model", grass_quad_model);
            quad.draw();
        }

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

public:
    BlendingWindow(int width, int height)
        : Window(width, height, "Blending")
        , m_width(width)
        , m_height(height)
    {
        glEnable(GL_DEPTH_TEST);

        // For the below two lines two work properly we need to sort the objects based on their distance to the camera and 
        // then draw starting from the farthest one so that all ojects are rendered correctly (The depth buffer does't care 
        // about transparency)
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        set_mouse_capture(true);

        grass_texture.texture_from_file("grass.png");

        m_camera.set_screen_size(width, height);
        m_camera.mouse_data.first_mouse = true;
        m_camera.m_position = glm::vec3(0.0f, 1.0f, 3.0f);

        plane.load(10.0f, 10.0f, 10, 13);
        plane_transform
            .translate(glm::vec3(-5.0f, 0.0f, 5.0f))
            .rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        x_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
        y_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
        z_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f));

        vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
        vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
        vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
        vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
        vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

        grass_quad_shader.set_shader_code(
            R"(
                #version 330 core
                layout (location = 0) in vec3 position;
                layout (location = 1) in vec3 normal;
                layout (location = 2) in vec2 tex_coord;

                uniform mat4 model;
                uniform mat4 view;
                uniform mat4 projection;

                out vec2 texCoord;

                void main() {
                    gl_Position = projection * view * model * vec4(position, 1.0);
                    texCoord = tex_coord;
                }
            )",

            R"(
                #version 330 core

                out vec4 frag_color;
                in vec2 texCoord;
                uniform sampler2D texture1;

                void main() {
                    vec4 tex_color = texture(texture1, texCoord);
                    if (tex_color.a < 0.1)
                        discard;
                    frag_color = tex_color;
                }
            )");

        check_for_opengl_error();
    }
};

// int main(int argc, char** argv)
// {
//     stbi_set_flip_vertically_on_load(true);

//     BlendingWindow w(800, 600);
//     w.start();

//     return 0;
// }

// /* ray tracer
//  * fogs
//  * sky box
//  * text
//  */