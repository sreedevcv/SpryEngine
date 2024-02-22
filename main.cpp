#include <iostream>
#include <cmath>

#include "Entity.hpp"
#include "Mat4.hpp"
#include "Vec3.hpp"
#include "Transforms.hpp"

#include "Cuboid.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Tetrahedron.hpp"
#include "Camera.hpp"

#include "utils.hpp"

class MyWindow : public spry::Window {
private:
    int m_width = 600;
    int m_height = 400;
    spry::Shader mShader;
    spry::Tetrahedron cube;
    spry::Camera m_camera;

protected:
    void updateFrame(float deltaTime)
    {
        auto model = spry::Mat4<float>(1.0f);
        auto view = m_camera.get_view_matrix();
        auto projection = m_camera.projection;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mShader.use();
        mShader.setUniformMatrix("model", model);
        mShader.setUniformMatrix("view", view);
        mShader.setUniformMatrix("projection", projection);
        cube.draw();

        check_for_opengl_error();
    }

    void processInput(float deltaTime)
    {
        const auto printMat = [](const spry::Mat4<float>& m) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                std::cout << m.values[i * 4 + j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    };
        if (isPressed(GLFW_KEY_ESCAPE)) {
            closeWindow();
        }

        if (isPressed(GLFW_KEY_W)) {
            m_camera.process_movement(spry::Camera::movement::FORWARD, deltaTime);
std::cout << m_camera.m_position.x << " " << m_camera.m_position.y << " " << m_camera.m_position.z << "\n";
            printMat(m_camera.get_view_matrix());
        }
        if (isPressed(GLFW_KEY_S)) {
            m_camera.process_movement(spry::Camera::movement::BACKWARD, deltaTime);
        }
        if (isPressed(GLFW_KEY_A)) {
            m_camera.process_movement(spry::Camera::movement::LEFT, deltaTime);
        }
        if (isPressed(GLFW_KEY_D)) {
            m_camera.process_movement(spry::Camera::movement::RIGHT, deltaTime);
        }
    }

public:
    MyWindow(int width, int height)
        : Window(width, height, "Test")
        , m_width(width)
        , m_height(height)
        , mShader("C:/Users/SHARIHA3/source/repos/SpryEngine/basic.vert", "C:/Users/SHARIHA3/source/repos/SpryEngine/basic.frag")
        , cube(mShader)
        , m_camera(width, height)
    {
        glClearColor(0.2f, 0.1f, 0.23f, 1.0f);
        mShader.compile();
    }
};

int main(int argc, char** argv)
{
    

    MyWindow w(600, 400);
    w.start();

    return 0;
}