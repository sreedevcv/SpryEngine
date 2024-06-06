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
#include "Sphere.hpp"

#include "utils.hpp"

class TextureWindow : public spry::Window {
private:
    int m_width = 600;
    int m_height = 400;

    spry::Shader shader = spry::ShaderManager::mvp_shader();
    spry::Shader quad_texture_shader;
    spry::Camera m_camera;
    spry::Line x_axis;
    spry::Line y_axis;
    spry::Line z_axis;
    spry::Cuboid cube;
    spry::Sphere sphere;
    spry::PlaneMesh plane;
    spry::Transform plane_transform;
    spry::Quad quad;

    unsigned int frame_buffer;
    unsigned int screen_texture;

protected:
    void update_frame(float delta_time) override
    {
        process_input(delta_time);
        // First pass
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
        glClearColor(0.4f, 0.5f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Drawing First Scene
        auto model = glm::mat4(1.0f);
        auto view = glm::lookAt(glm::vec3(1.0f, 1.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        auto projection = m_camera.get_projection_matrix();

        shader.use();
        shader.set_uniform_matrix("projection", projection);
        shader.set_uniform_matrix("view", view);
        draw_axes();

        shader.set_uniform_vec("color", glm::vec4(0.9f, 0.7f, 0.9f, 1.0f));
        shader.set_uniform_matrix("model", spry::Transform().rotate(get_global_time(), glm::vec3(1.0f, 1.0f, 0.0f)).translate(glm::vec3(1.0f, 1.0f, 1.0f)).get_model());
        sphere.draw();
        check_for_opengl_error();

        // Second Pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        view = m_camera.get_view_matrix();
        quad_texture_shader.use();
        quad_texture_shader.set_uniform_matrix("view", view);
        quad_texture_shader.set_uniform_matrix("projection", projection);
        quad_texture_shader.set_uniform_matrix("model", spry::Transform().get_model());
        glBindTexture(GL_TEXTURE_2D, screen_texture);
        quad.draw();

        shader.use();
        shader.set_uniform_matrix("view", view);
        shader.set_uniform_vec("color", glm::vec4(0.5f, 0.4f, 0.4f, 1.0f));
        shader.set_uniform_matrix("model", plane_transform.get_model());
        plane.draw();

        check_for_opengl_error();
        std::cout << 1.0f / delta_time << "\n";
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
    TextureWindow(int width, int height)
        : Window(width, height, "Rendering To Texture")
        , m_width(width)
        , m_height(height)
    {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        set_mouse_capture(true);

        m_camera.set_screen_size(width, height);
        m_camera.mouse_data.first_mouse = true;
        m_camera.m_position = glm::vec3(0.0f, 1.0f, 3.0f);

        plane.load(10.0f, 10.0f, 10, 13);
        sphere.load(1.0f, 20, 20);
        plane_transform
            .translate(glm::vec3(-5.0f, -5.0f, 5.0f))
            .rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        x_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
        y_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
        z_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f));

        glGenFramebuffers(1, &frame_buffer);
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

        glGenTextures(1, &screen_texture);
        glBindTexture(GL_TEXTURE_2D, screen_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_texture, 0);

        unsigned int render_buffer;
        glGenRenderbuffers(1, &render_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, render_buffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete\n";
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        quad_texture_shader.set_shader_code(
            R"(
                #version 330 core
                layout (location = 0) in vec3 position;
                layout (location = 1) in vec3 normal;
                layout (location = 2) in vec2 tex_coord;

                out vec2 texCoord;

                uniform mat4 model;
                uniform mat4 view;
                uniform mat4 projection;

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
                    frag_color = tex_color;
                }
            )");

        check_for_opengl_error();
    }
};

// int main(int argc, char** argv)
// {
//     stbi_set_flip_vertically_on_load(true);

//     TextureWindow w(800, 600);
//     w.start();

//     return 0;
// }
