#include <iostream>
#include <vector>
#include <fstream>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

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
    spry::CubeMap skybox;

    spry::Point points;
    spry::Shader point_shader;

    bool update_camera = true;

    spry::Shader explode_shader;

protected:
    void update_frame(float delta_time) override
    {
        process_input(delta_time);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        auto model = glm::mat4(1.0f);
        auto view = m_camera.get_view_matrix();
        auto projection = m_camera.get_projection_matrix();

        shader.use();
        shader.set_uniform_matrix("projection", projection);
        shader.set_uniform_matrix("view", view);
        draw_axes();


        point_shader.use();
        point_shader.set_uniform_matrix("model", model);
        point_shader.set_uniform_matrix("projection", projection);
        point_shader.set_uniform_matrix("view", view);
        points.draw();

        explode_shader.use();
        explode_shader.set_uniform_matrix("model", model);
        explode_shader.set_uniform_matrix("projection", projection);
        explode_shader.set_uniform_matrix("view", view);
        explode_shader.set_uniform_vec("color", glm::vec4(1.0f, 0.5f, 0.8f, 1.0f));
        explode_shader.set_uniform_float("time", get_global_time());
        sphere.draw();

        check_for_opengl_error();
        // close_window();
        draw_ui(delta_time);
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
        if (is_key_pressed(GLFW_KEY_U)) {
            set_mouse_capture(false);
            update_camera = false;
        }
        if (is_key_pressed(GLFW_KEY_L)) {
            set_mouse_capture(true);
            update_camera = true;
            m_camera.mouse_data.first_mouse = true;
        }
    }

    void on_mouse_move(double x_pos_in, double y_pos_in) override
    {
        if (!update_camera)
            return;
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

    void draw_ui(float delta_time)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ImGui::ShowDemoWindow();
        ImGui::Begin("Options");
        ImGui::Text("FPS: %f", 1.0f / delta_time);
        ImGui::Text("Pos: %f, %f %f", m_camera.m_position.x, m_camera.m_position.y, m_camera.m_position.z);
        // ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
        // ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("speed", &m_camera.m_speed, 0.0f, 1000.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        // ImGui::ColorEdit3("clear color", (float*&clear_color); // Edit 3 floats representing a color

        // ImGui::Text("counter =%d", counter);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

public:
    MyWindow(int width, int height)
        : Window(width, height, "Point Rendering")
        , m_width(width)
        , m_height(height)
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearColor(0.3, 0.3, 0.3, 1.0f);

        set_mouse_capture(true);
        glPointSize(1.0f);

        m_camera.set_screen_size(width, height);
        m_camera.mouse_data.first_mouse = true;

        m_camera.m_far_point = 2000.0f;

        x_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
        y_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
        z_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f));

        sphere.load(1.0f, 20, 20);

        std::vector<glm::vec3> point_data = {
            { 1.0f, 1.0f, 0.0f },
            { 1.0f, -1.0f, 0.0f },
            { -1.0f, 1.0f, 0.0f },
            // {-1.0f, -1.0f, 0.0f},
        };

        points.load(point_data);

        point_shader.set_shader_code(
            R"(
                #version 460 core
                layout (location = 0) in vec3 position;

                uniform mat4 model;
                uniform mat4 view;
                uniform mat4 projection;

                void main() {
                    gl_Position = projection * view * model * vec4(position, 1.0);
                }
            )",
            R"(
                #version 460 core
                out vec4 frag_color;

                void main() {
                    frag_color = vec4(1.0f, 0.9f, 0.9f, 1.0f);
                }
            )",

            R"(
                #version 460 core
                layout (points) in;
                layout (line_strip, max_vertices=2) out;

                void main() {
                    gl_Position = gl_in[0].gl_Position + vec4(-1.0, 0.0, 0.0, 0.0);
                    EmitVertex();
                    gl_Position = gl_in[0].gl_Position + vec4(1.0, 0.0, 0.0, 0.0);
                    EmitVertex();

                    EndPrimitive();
                }
            )");

        explode_shader.set_shader_code(
            R"(
                #version 330
                layout (location = 0) in vec3 apos;

                uniform mat4 model;
                uniform mat4 view;
                uniform mat4 projection;

                void main() {
                    gl_Position = projection * view * model * vec4(apos, 1.0);
                }
            )",
            R"(
                #version 330 core 

                out vec4 frag_color;

                uniform vec4 color;
                
                void main() {
                    frag_color = color;
                }
            )",
            R"(
                #version 330 core
                layout (triangles) in;
                layout (triangle_strip, max_vertices=3) out;

                uniform float time;

                vec3 get_normal() {
                    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
                    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
                    return normalize(cross(a, b));
                }

                vec4 explode(vec4 position, vec3 normal) {
                    float magnitude = 2.0;
                    vec3 direction = normal * (sin(time) + 1.0) / 2.0 * magnitude;
                    return position + vec4(direction, 0.0);
                }

                void main() {
                    vec3 normal = get_normal();
                    gl_Position = explode(gl_in[0].gl_Position, normal);
                    EmitVertex();
                    gl_Position = explode(gl_in[1].gl_Position, normal);
                    EmitVertex();
                    gl_Position = explode(gl_in[2].gl_Position, normal);
                    EmitVertex();
                    EndPrimitive();
                }
            )"
        );

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        ImGui::StyleColorsLight();
        ImGui_ImplGlfw_InitForOpenGL(get_window(), true);
        ImGui_ImplOpenGL3_Init();

        check_for_opengl_error();
    }
};

// int main(int argc, char** argv)
// {
//     // stbi_set_flip_vertically_on_load(true);

//     MyWindow w(1200, 1000);
//     w.start();

//     return 0;
// }