#pragma once

#include "Vec3.hpp"
#include "Mat4.hpp"

#define check_for_opengl_error()                                                             \
    while (GLenum error = glGetError()) {                                                    \
        std::cout << __FILE__ << ":" << __LINE__ << " OpenGL Error: " << error << std::endl; \
    }

struct Color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
};

consteval float norm_color(int color)
{
    return static_cast<float>(color) / 255.0f;
}

const auto printMat = [](const spry::Mat4<float>& m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << m.values[i * 4 + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
};

const auto printVec = [](const spry::Vec3<float>& m) {
    std::cout << m.x << " " << m.y << " " << m.z << "\n";
};