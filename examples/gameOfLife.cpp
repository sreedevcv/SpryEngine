#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

#include <cqueue.hpp>

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
#include "Model.hpp"

#include "utils.hpp"
#include "CubeMap.hpp"

class GameOfLife {
private:
    int height = 30;
    int width = 30;
    int depth = 30;
    float offset = 0.1f;
    float cube_size = 1.0f;
    spry::Cuboid cube;
    float interval = 0;

    bool* board1;
    bool* board2;

    gto::cqueue<bool*> queue;

    int get_neighbour_count(int x, int y)
    {
        int count = 0;
        if (x > 0 && board1[(x - 1) * width + y]) {
            count += 1;
        }
        if (y > 0 && board1[x * width + (y - 1)]) {
            count += 1;
        }
        if (x > 0 && y > 0 && board1[(x - 1) * width + (y - 1)]) {
            count += 1;
        }

        if (x < height - 1 && board1[(x + 1) * width + y]) {
            count += 1;
        }
        if (y < width - 1 && board1[x * width + (y + 1)]) {
            count += 1;
        }
        if (x < height && y < width - 1 && board1[(x + 1) * width + (y + 1)]) {
            count += 1;
        }

        if (x > 0 && y < width - 1 && board1[(x - 1) * width + (y + 1)]) {
            count += 1;
        }
        if (y > 0 && x < height - 1 && board1[(x + 1) * width + (y - 1)]) {
            count += 1;
        }
        return count;
    }

public:
    GameOfLife()
    {
        for (int i = 0; i < depth; i++) {
            queue.push_back(new bool[width * height]);
        }

        for (int k = 0; k < depth; k++) {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    queue[k][i * width + j] = rand() % 2 == 0 ? true : false;
                }
            }
        }
    }

    ~GameOfLife()
    {
        for (int i = 0; i < depth; i++) {
            delete[] queue[i];
        }
    }

    void draw(spry::Shader& shader)
    {
        for (int k = 0; k < depth; k++) {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    if (queue[k][i * width + j]) {
                        float x = i * (1 + offset) * cube_size;
                        float y = depth * (1 +  offset) * cube_size - (k * (1 + offset) * cube_size) - (i + j) * 0.2;
                        float z = j * (1 + offset) * cube_size;
                        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

                        auto model = spry::Transform()
                                         .translate(glm::vec3(x, y, z))
                                         .scale(glm::vec3(cube_size, cube_size, cube_size))
                                         .get_model();
                        shader.set_uniform_vec("object_color", color - ((1.0f / depth) * k));
                        shader.set_uniform_matrix("model", model);
                        cube.draw();
                    }
                }
            }
        }
    }

    void next_state(float delta)
    {
        interval += delta;

        if (interval < 0.5f) {
            return;
        }
        interval = 0;

        board1 = queue.front();
        board2 = queue.back();

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int count = get_neighbour_count(i, j);

                if (board1[i * width + j] && count < 2) {
                    board2[i * width + j] = false;
                } else if (board1[i * width + j] && (count == 2 || count == 3)) {
                    board2[i * width + j] = true;
                } else if (board1[i * width + j] && count > 2) {
                    board2[i * width + j] = false;
                } else if (!board1[i * width + j] && count == 3) {
                    board2[i * width + j] = true;
                } else {
                    board2[i * width + j] = board1[i * width + j];
                }
            }
        }

        bool* back = queue.back();
        queue.pop_back();
        queue.push_front(back);
    }
};

class GOLWindow : public spry::Window {
private:
    int m_width = 600;
    int m_height = 400;

    spry::Shader shader = spry::ShaderManager::mvp_shader();
    spry::Shader cube_shader;
    spry::Camera m_camera;
    spry::Line x_axis;
    spry::Line y_axis;
    spry::Line z_axis;
    spry::Cuboid cube;
    spry::Sphere sphere;
    spry::PlaneMesh plane;
    GameOfLife gol;

    glm::vec3 light_pos = glm::vec3(20.0f, 40.0f, 40.0f);
    // glm::vec3 light_color = glm::vec3(0.6f, 0.3f, 0.4f);
    glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuse_color = light_color * glm::vec3(0.5f);
    glm::vec3 ambient_color = diffuse_color * glm::vec3(0.2f);
    glm::vec4 object_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    struct Material {
        // glm::vec3 ambient = glm::vec3(1.0f, 0.5f, 0.31f);
        // glm::vec3 diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
        // glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
        float shininess = 32.0f;
    } material;

    bool update_camera = true;

protected:
    void update_frame(float delta_time) override
    {
        process_input(delta_time);

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        auto model = glm::mat4(1.0f);
        auto view = m_camera.get_view_matrix();
        auto projection = m_camera.get_projection_matrix();

        shader.use();
        shader.set_uniform_matrix("model", model);
        shader.set_uniform_matrix("projection", projection);
        shader.set_uniform_matrix("view", view);
        draw_axes();

        cube_shader.use();
        cube_shader.set_uniform_vec("material.ambient", material.ambient);
        cube_shader.set_uniform_vec("material.diffuse", material.diffuse);
        cube_shader.set_uniform_vec("material.specular", material.specular);
        cube_shader.set_uniform_float("material.shininess", material.shininess);

        cube_shader.set_uniform_vec("light.position", light_pos);
        cube_shader.set_uniform_vec("light.ambient", ambient_color);
        cube_shader.set_uniform_vec("light.diffuse", diffuse_color);
        cube_shader.set_uniform_vec("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        cube_shader.set_uniform_vec("object_color", object_color);
        cube_shader.set_uniform_vec("view_pos", m_camera.m_position);

        cube_shader.set_uniform_matrix("projection", projection);
        cube_shader.set_uniform_matrix("view", view);

        gol.draw(cube_shader);
        gol.next_state(delta_time);

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
        ImGui::Begin("Options");
        ImGui::Text("FPS: %f", 1.0f / delta_time);
        ImGui::Text("Pos: %f, %f %f", m_camera.m_position.x, m_camera.m_position.y, m_camera.m_position.z);
        ImGui::SliderFloat("speed", &m_camera.m_speed, 0.0f, 1000.0f);
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

public:
    GOLWindow(int width, int height)
        : Window(width, height, "Trees")
        , m_width(width)
        , m_height(height)
        , cube_shader("../examples/lighting.vert", "../examples/lighting.frag")
    // , tree("../examples/models/backpack/backpack.obj")
    // , tmodel("../examples/models/backpack/backpack.obj")
    {
        srand(time(0));
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glCullFace(GL_BACK);
        glClearColor(0.3, 0.3, 0.3, 1.0f);

        set_mouse_capture(true);
        glPointSize(1.0f);

        m_camera.set_screen_size(width, height);
        m_camera.mouse_data.first_mouse = true;
        m_camera.m_far_point = 1000.0f;
        m_camera.set_position(glm::vec3(9.0f, 14.0f, 44.0f));

        x_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1000.0f, 0.0f, 0.0f));
        y_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1000.0f, 0.0f));
        z_axis.set_end_points(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1000.0f));

        sphere.load(1.0f, 20, 20);
        plane.load(100.0f, 100.0f, 10, 10);

        cube_shader.compile();

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();
        ImGui_ImplGlfw_InitForOpenGL(get_window(), true);
        ImGui_ImplOpenGL3_Init();

        check_for_opengl_error();
    }
};

int main(int argc, char** argv)
{
    // stbi_set_flip_vertically_on_load(true);

    GOLWindow w(1200, 1000);
    w.start();
    return 0;
}