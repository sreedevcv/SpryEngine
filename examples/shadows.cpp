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
    spry::PlaneMesh plane;
    spry::Quad quad;
    bool update_camera = true;

    spry::Shader simple_depth_shader;
    spry::Shader quad_shader;
    spry::Shader shadow_shader;
    const uint32_t SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    uint32_t depth_map_fbo;
    uint32_t depth_map;

    float near_plane = 1.0f;
    float far_plane = 20.0f;
    float b1 = 0.05;
    float b2 = 0.005;

protected:
    void update_frame(float delta_time) override
    {
        process_input(delta_time);

        glm::mat4 light_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        glm::vec3 light_pos = glm::vec3(2.0f * sinf(get_global_time()), 4.0f, -2.0f * sinf(get_global_time()));
        glm::mat4 light_view = glm::lookAt(light_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 light_space_matrix = light_projection * light_view;

        simple_depth_shader.use();
        simple_depth_shader.set_uniform_matrix("light_space_matrix", light_space_matrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
        glCullFace(GL_FRONT);
        glClear(GL_DEPTH_BUFFER_BIT);
        render_scene(simple_depth_shader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        check_for_opengl_error();

        glViewport(0, 0, m_width, m_height);
        glCullFace(GL_BACK);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // quad_shader.use();
        // quad_shader.set_uniform_matrix("model", glm::mat4(1.0f));
        // quad_shader.set_uniform_matrix("view", m_camera.get_view_matrix());
        // quad_shader.set_uniform_matrix("projection", m_camera.get_projection_matrix());
        // glBindTexture(GL_TEXTURE_2D, depth_map);
        // quad.draw();
        check_for_opengl_error();

        shadow_shader.use();
        shadow_shader.set_uniform_matrix("view", m_camera.get_view_matrix());
        shadow_shader.set_uniform_matrix("projection", m_camera.get_projection_matrix());
        shadow_shader.set_uniform_matrix("light_space_matrix", light_space_matrix);
        shadow_shader.set_uniform_vec("light_pos", light_pos);
        shadow_shader.set_uniform_vec("view_pos", m_camera.m_position);
        shadow_shader.set_uniform_float("b1", b1);
        shadow_shader.set_uniform_float("b2", b2);
        glBindTexture(GL_TEXTURE_2D, depth_map);

        render_scene(shadow_shader, true);

        // close_window();
        draw_ui(delta_time);
    }

    void render_scene(spry::Shader& shader, bool use_color = false)
    {
        auto model = glm::mat4(1.0f);
        shader.set_uniform_matrix("model", spry::Transform().translate(glm::vec3(0.0f, 2.0f * sinf(get_global_time()), 1.0f)).get_model());
        if (use_color)
            shader.set_uniform_vec("color", glm::vec4(0.5f, 0.3f, 0.3f, 1.0f));
        sphere.draw();

        shader.set_uniform_matrix("model", spry::Transform().translate(glm::vec3(-50.0f, 0.0f, 50.0f)).rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)).get_model());
        if (use_color)
            shader.set_uniform_vec("color", glm::vec4(0.3f, 0.5f, 0.3f, 1.0f));
        plane.draw();

        shader.set_uniform_matrix("model", spry::Transform().rotate(get_global_time(), glm::vec3(0.0f, 1.0f, 0.0f)).translate(glm::vec3(1.0f, 3.0f, 0.0f)).get_model());
        if (use_color)
            shader.set_uniform_vec("color", glm::vec4(0.3f, 0.3f, 0.5f, 1.0f));
        cube.draw();

        shader.set_uniform_matrix("model", spry::Transform().translate(glm::vec3(-4.0f, 2.0f, 3.0f)).rotate(get_global_time(), glm::vec3(1.0f, 1.0f, 0.0f)).get_model());
        if (use_color)
            shader.set_uniform_vec("color", glm::vec4(0.3f, 0.3f, 0.5f, 1.0f));
        cube.draw();
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

        ImGui::SliderFloat("speed", &m_camera.m_speed, 0.0f, 1000.0f);
        ImGui::SliderFloat("near_plane", &near_plane, 0.0f, 1000.0f);
        ImGui::SliderFloat("far_plane", &far_plane, 0.0f, 1000.0f);
        ImGui::SliderFloat("b1", &b1, 0.0f, 0.2f);
        ImGui::SliderFloat("b2", &b2, 0.0f, 0.2f);
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
        glClearColor(0.8, 0.8, 0.8, 1.0f);

        set_mouse_capture(true);
        glPointSize(1.0f);

        m_camera.set_screen_size(width, height);
        m_camera.mouse_data.first_mouse = true;
        m_camera.m_far_point = 2000.0f;

        x_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
        y_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
        z_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f));

        sphere.load(1.0f, 20, 20);
        plane.load(100.0f, 100.0f, 1, 1);

        glGenFramebuffers(1, &depth_map_fbo);

        // Create a shadow map
        glGenTextures(1, &depth_map);
        glBindTexture(GL_TEXTURE_2D, depth_map);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        // Attach it to the depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        ImGui::StyleColorsLight();
        ImGui_ImplGlfw_InitForOpenGL(get_window(), true);
        ImGui_ImplOpenGL3_Init();

        simple_depth_shader.set_shader_code(
            R"(
                #version 460 core
                layout (location = 0) in vec3 pos;

                uniform mat4 light_space_matrix;
                uniform mat4 model;

                void main() {
                    gl_Position = light_space_matrix * model * vec4(pos, 1.0);
                }
            )",
            R"(
                #version 460 core
                void main() {
                    // gl_FragDepth = gl_FragCoord.z;
                }
            )");

        quad_shader.set_shader_code(
            R"(
                #version 460 core
                layout (location = 0) in vec3 pos;
                layout (location = 1) in vec3 normal;
                layout (location = 2) in vec2 texcoords;

                uniform mat4 projection;
                uniform mat4 view;
                uniform mat4 model;

                out vec2 tex_coords;

                void main() {
                    gl_Position = projection * view * model * vec4(pos, 1.0);
                    tex_coords = texcoords;
                }
            )",
            R"(
                #version 460 core
                out vec4 frag_color;
                in vec2 tex_coords;
                uniform sampler2D depth_map;

                void main() {
                    float depth_value = texture(depth_map, tex_coords).r;
                    frag_color = vec4(vec3(depth_value), 1.0);
                }
            )");

        shadow_shader.set_shader_code(
            R"(
                #version 460 core
                layout (location = 0) in vec3 pos;
                layout (location = 1) in vec3 normal;

                out VS_OUT {
                    vec3 frag_pos;
                    vec3 normal;
                    vec4 frag_pos_light_space;
                } vs_out;

                uniform mat4 projection;
                uniform mat4 view;
                uniform mat4 model;
                uniform mat4 light_space_matrix;

                void main() {
                    vs_out.frag_pos = vec3(model * vec4(pos, 1.0));
                    vs_out.normal = transpose(inverse(mat3(model))) * normal;
                    vs_out.frag_pos_light_space = light_space_matrix * vec4(vs_out.frag_pos, 1.0);
                    gl_Position = projection * view * vec4(vs_out.frag_pos, 1.0);
                }

            )",
            R"(
                #version 330 core
                out vec4 frag_color;

                in VS_OUT {
                    vec3 frag_pos;
                    vec3 normal;
                    vec4 frag_pos_light_space;
                } fs_in;
                
                uniform sampler2D shadow_map;
                uniform vec3 light_pos;
                uniform vec3 view_pos;
                uniform vec4 color;

                uniform float b1;
                uniform float b2;

                float calculate_shadow(vec4 frag_pos_light_pos, float bias) {
                    vec3 proj_coords = frag_pos_light_pos.xyz / frag_pos_light_pos.w;
                    proj_coords = proj_coords * 0.5 + 0.5;

                    if(proj_coords.z > 1.0)
                        return 0.0;

                    float closest_depth = texture(shadow_map, proj_coords.xy).r;
                    float current_depth = proj_coords.z;
                    float shadow = 0.0;
                    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);

                    int radius = 2;
                    for(int x = -radius; x <= radius; ++x)
                    {
                        for(int y = -radius; y <= radius; ++y)
                        {
                            float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r; 
                            shadow += (current_depth - bias) > pcf_depth ? 1.0 : 0.0;        
                        }    
                    }

                    shadow /= pow(2 * radius + 1, 2);
                    return shadow;
                }

                void main() {
                    vec3 normal = normalize(fs_in.normal);
                    vec3 light_color = vec3(1.0);

                    vec3 ambient = 0.15 * light_color;
                    
                    vec3 light_dir = normalize(light_pos - fs_in.frag_pos);
                    float diff = max(dot(light_dir, normal), 0.0);
                    vec3 diffuse = diff * light_color;

                    vec3 view_dir = normalize(view_pos - fs_in.frag_pos);
                    float spec = 0.0;
                    vec3 halfway_dir = normalize(light_dir + view_dir);
                    spec = pow(max(dot(normal, halfway_dir), 0.0), 64.0);
                    vec3 specular = spec * light_color;

                    float bias = max(b1 * (1.0 - dot(normal, light_dir)), b2);
                    float shadow = calculate_shadow(fs_in.frag_pos_light_space, bias);
                    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color.xyz;

                    frag_color = vec4(lighting, 1.0);
                }
            )");

        check_for_opengl_error();
    }
};

int main(int argc, char** argv)
{
    // stbi_set_flip_vertically_on_load(true);

    MyWindow w(1200, 1000);
    w.start();

    return 0;
}