#pragma once

#include <span>

#include <glad/glad.h>

#include "Shader.hpp"

namespace spry {

class BasicMesh {
private:
    int m_vertex_count;

    bool m_data_loaded = false;
    bool index_array = false;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    void delete_buffers();

public:
    BasicMesh();
    BasicMesh(std::span<float> vertices, std::span<int> indices, std::span<int> format);
    BasicMesh(std::span<float> vertices, std::span<int> format);
    ~BasicMesh();

    void load_data(std::span<float> vertices, std::span<int> indices, std::span<int> format);
    void load_data(std::span<float> vertices, std::span<int> format);
    void draw(GLenum mode = GL_TRIANGLES);

    // void operator=(BasicMesh& mesh);
};

}
