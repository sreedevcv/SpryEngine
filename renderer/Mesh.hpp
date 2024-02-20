#pragma once

#include <span>

#include <glad/glad.h>

#include "Shader.hpp"

namespace spry {

class Mesh {
private:
    int mVertexCount;
    Shader& mShader;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

public:
    Mesh(Shader& shader, std::span<float> vertices, std::span<int> indices, std::span<int> format);
    ~Mesh();

    void draw();
};

}
