#pragma once

#include <glad/glad.h>

#include "Shader.hpp"
#include "utils.hpp"

namespace spry {

class Cuboid {
private:
    GLuint VAO, VBO, EBO;
    spry::Shader& m_shader;

    void initBuffers();

public:
    Cuboid(spry::Shader& shader);
    ~Cuboid();

    void draw();
};

}