#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "Window.hpp"
#include "Cuboid.hpp"
#include "Camera.hpp"
#include "Shader.hpp"

struct Cube {
    spry::Cuboid cube;
    glm::vec3 position;
    glm::vec4 color;
};

class GameWindow : public spry::Window {
private:
    int m_width;
    int m_height;
    spry::Cuboid m_cube;
    spry::Camera m_camera;
    spry::Shader m_shader;
    std::vector<Cube> cubes;

    void update_frame(float delta_time) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        process_input(delta_time);

        auto view = m_camera.get_view_matrix();
        auto proj = m_camera.get_projection_matrix();

        auto model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)get_global_time(), glm::vec3(1.0f, 1.0f, 0.0f));

        m_shader.use();
        m_shader.set_uniform_matrix("model", model);
        m_shader.set_uniform_matrix("view", view);
        m_shader.set_uniform_matrix("projection", proj);
        m_cube.draw();
    }

    void process_input(float delta_time)
    {
        if (is_key_pressed(GLFW_KEY_ESCAPE)) {
            close_window();
        }
    }

public:
    GameWindow(int width, int height)
        : Window(width, height, "Rubiks Cube")
        , m_width(width)
        , m_height(height)
        , m_shader("../test/basic.vert", "../test/basic.frag")
    {
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.9, 0.8, 0.7, 1.0);
        m_shader.compile();
        m_camera.set_screen_size(width, height);
        m_camera.set_position(glm::vec3(0.0f, 1.0f, 5.0f));

        const int cube_size = 3;

        for (int i = 0; i < cube_size * cube_size; i++) {
            Cube cube = {
                .position = glm::vec3()
            };
            cubes.push_back(cube);
        }

        for (int i = 0; i < 2 * cube_size * cube_size + 2 * cube_size * (cube_size - 2); i++) {
            
        }
    }
};

// int main(int argc, char** argv)
// {
//     GameWindow w(800, 600);
//     w.start();
//     return 0;
// }
