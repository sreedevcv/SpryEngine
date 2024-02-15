#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "Shader.hpp"
#include  "utils.hpp"

class Tetrahedron {
private:
    unsigned int VAO, VBO, EBO;

public:
    Shader basic_shader;

    Tetrahedron();
    ~Tetrahedron();

    void draw();
};