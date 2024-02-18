#pragma once

#include <glad/glad.h>

namespace spry {

class Texture {
private:
    GLuint texture;

public:
    Texture();
    ~Texture();

    void load_texture(void* data, int width, int height);
    void bind(int texCount);
};

}