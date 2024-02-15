#pragma once

#define check_for_opengl_error() \
    while (GLenum error = glGetError()) { \
        std::cout << __FILE__ << ":" << __LINE__ << " OpenGL Error: " << error << std::endl; \
    } \

struct Color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
};

consteval float norm_color(int color) {
    return static_cast<float>(color) / 255.0f;
}

struct Config {
    int world_X = 20;
    int world_Y = 20;
};
