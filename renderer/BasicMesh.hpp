#pragma once

#include <span>

#include <glad/glad.h>

#include "Shader.hpp"

namespace spry {

class BasicMesh {
private:
    int m_vertex_count;
    bool m_data_loaded = false;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

public:
    BasicMesh();
    BasicMesh(std::span<float> vertices, std::span<int> indices, std::span<int> format);
    ~BasicMesh();

    void load_data(std::span<float> vertices, std::span<int> indices, std::span<int> format);
    void draw();
};

}
