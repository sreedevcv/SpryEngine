#pragma once

#include <glad/glad.h>

#include <string>

namespace spry {

class Texture {
private:
    GLuint texture;

public:
    Texture();
    ~Texture();

    void load_texture(void* data, int width, int height);
    void bind(int texCount);

    static unsigned int texture_from_file(const char *path, const std::string &directory);
};

}