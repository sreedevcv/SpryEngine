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
#include "CubeMap.hpp"

class CubeMapWindow : public spry::Window {
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
    spry::CubeMap skybox;
    spry::Shader reflection_shader;
    spry::Shader refraction_shader;

protected:
    void update_frame(float delta_time) override
    {
        process_input(delta_time);

        glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        auto model = glm::mat4(1.0f);
        auto view = m_camera.get_view_matrix();
        auto projection = m_camera.get_projection_matrix();

        shader.use();
        shader.set_uniform_matrix("projection", projection);
        shader.set_uniform_matrix("view", view);
        draw_axes();

        check_for_opengl_error();

        // shader.set_uniform_vec("color", glm::vec4(0.9f, 0.7f, 0.7f, 1.0f));
        reflection_shader.use();
        check_for_opengl_error();
        reflection_shader.set_uniform_matrix("model", spry::Transform().rotate(get_global_time(), glm::vec3(1.0f, 1.0f, 0.0f)).translate(glm::vec3(1.0f, 1.0f, 1.0f)).get_model());
        reflection_shader.set_uniform_matrix("view", view);
        reflection_shader.set_uniform_matrix("projection", projection);
        reflection_shader.set_uniform_vec("camera_pos", m_camera.m_position);
        sphere.draw();

        check_for_opengl_error();

        refraction_shader.use();
        refraction_shader.set_uniform_matrix("model", spry::Transform().translate(glm::vec3(-3.0f, 0.0f, 0.0f)).get_model());
        refraction_shader.set_uniform_matrix("view", view);
        refraction_shader.set_uniform_matrix("projection", projection);
        refraction_shader.set_uniform_vec("camera_pos", m_camera.m_position);
        cube.draw();

        skybox.draw(glm::mat4(glm::mat3(view)), projection);

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
    CubeMapWindow(int width, int height)
        : Window(width, height, "Rendering To Texture")
        , m_width(width)
        , m_height(height)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        set_mouse_capture(true);

        m_camera.set_screen_size(width, height);
        m_camera.mouse_data.first_mouse = true;
        m_camera.m_position = glm::vec3(0.0f, 1.0f, 3.0f);

        x_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
        y_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
        z_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f));

        sphere.load(1.0f, 20, 20);

        const char* paths[] = {
            "../examples/skybox/right.jpg",
            "../examples/skybox/left.jpg",
            "../examples/skybox/top.jpg",
            "../examples/skybox/bottom.jpg",
            "../examples/skybox/front.jpg",
            "../examples/skybox/back.jpg",
        };
        skybox.load_cube_map(paths);

        const char* vertex_shader = R"(
                #version 330 core
                layout (location = 0) in vec3 position;
                layout (location = 1) in vec3 normal;

                out vec3 pos;
                out vec3 norm;

                uniform mat4 model;
                uniform mat4 view;
                uniform mat4 projection;

                void main() {
                    pos = vec3(model * vec4(position, 1.0));
                    norm = mat3(transpose(inverse(model))) * normal;
                    gl_Position = projection * view * model * vec4(position, 1.0);
                }
            )";

        reflection_shader.set_shader_code(
            vertex_shader,
            R"(
                #version 330 core
                out vec4 frag_color;

                in vec3 pos;
                in vec3 norm;

                uniform vec3 camera_pos;
                uniform samplerCube skybox;

                void main() {
                    vec3 incident = normalize(pos - camera_pos);
                    vec3 reflected = reflect(incident, normalize(norm));
                    frag_color = vec4(texture(skybox, reflected).rgb, 1.0);
                }
            )");

        refraction_shader.set_shader_code(
            vertex_shader,
            R"(
                    #version 330 core
                    out vec4 frag_color;

                    in vec3 pos;
                    in vec3 norm;

                    uniform vec3 camera_pos;
                    uniform samplerCube skybox;

                    void main() {
                        float ratio = 1.0 / 1.52;
                        vec3 incident = normalize(pos - camera_pos);
                        vec3 refracted = refract(incident, normalize(norm), ratio);
                        frag_color = vec4(texture(skybox, refracted).rgb, 1.0);
                    }
                )");

        check_for_opengl_error();
    }
};

// int main(int argc, char** argv)
// {
//     // stbi_set_flip_vertically_on_load(true);

//     CubeMapWindow w(800, 600);
//     w.start();

//     return 0;
// }