#pragma once

#include <glad/glad.h>

#include "Shader.hpp"
#include "utils.hpp"

namespace spry {

class Tetrahedron {
private:
    unsigned int VAO, VBO, EBO;
    
    void initBuffers();

public:
    spry::Shader& m_shader;

    Tetrahedron(Shader& shader);
    ~Tetrahedron();

    void draw();
};

}