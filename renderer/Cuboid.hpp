#pragma once

#include <glad/glad.h>

#include "Shader.hpp"
#include "utils.hpp"

class Cuboid {
private:
    GLuint VAO, VBO, EBO;

public:
    Shader basic_shader;

    Cuboid();
    ~Cuboid();

    void draw();
};