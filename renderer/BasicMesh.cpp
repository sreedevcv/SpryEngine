#include "BasicMesh.hpp"

spry::BasicMesh::BasicMesh()
    : m_data_loaded(false)
{
}

spry::BasicMesh::BasicMesh(Shader* shader, std::span<float> vertices, std::span<int> indices, std::span<int> format)
    : m_vertex_count(indices.size())
    , m_shader(shader)
{
    load_data(shader, vertices, indices, format);
    m_data_loaded = true;
}

spry::BasicMesh::~BasicMesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
}

void spry::BasicMesh::load_data(Shader *shader, std::span<float> vertices, std::span<int> indices, std::span<int> format)
{
    if (m_data_loaded) {
        return;
    }

    m_vertex_count = indices.size();
    m_shader = shader;
    m_data_loaded = true;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), (void*)vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), (void*)indices.data(), GL_STATIC_DRAW);

    int stride = 0;
    for (int attribSize : format) {
        stride += attribSize;
    }

    long prevAttribSize = 0;
    for (int i = 0; i < format.size(); i++) {
        int attribSize = format[i];
        glVertexAttribPointer(i, attribSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(prevAttribSize * sizeof(float)));
        glEnableVertexAttribArray(i);
        prevAttribSize += attribSize;
    }
}

void spry::BasicMesh::draw()
{
    m_shader->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, m_vertex_count, GL_UNSIGNED_INT, 0);
}
