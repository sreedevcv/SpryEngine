#pragma once

#include <glad/glad.h>

#include "Shader.hpp"
#include "utils.hpp"

namespace spry {

class Cuboid {
private:
    GLuint VAO, VBO, EBO;

    void initBuffers();

public:
    Cuboid();
    ~Cuboid();

    void draw();
};

}