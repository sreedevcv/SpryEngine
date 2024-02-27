#include "BasicMesh.hpp"

spry::BasicMesh::BasicMesh()
    : m_data_loaded(false)
{
}

spry::BasicMesh::BasicMesh(std::span<float> vertices, std::span<int> indices, std::span<int> format)
    : m_vertex_count(indices.size())
{
    load_data(vertices, indices, format);
}

spry::BasicMesh::BasicMesh(std::span<float> vertices, std::span<int> format)
{
    load_data(vertices, format);
}

void spry::BasicMesh::delete_buffers()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    if (index_array) {
        glDeleteBuffers(1, &EBO);
    }
}


spry::BasicMesh::~BasicMesh()
{
    delete_buffers();
}

void spry::BasicMesh::load_data(std::span<float> vertices, std::span<int> indices, std::span<int> format)
{
    if (m_data_loaded) {
        // std::cout << "Cannot reload data\n";
        // return;
        delete_buffers();
    }

    m_vertex_count = indices.size();
    m_data_loaded = true;
    index_array = true;

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

void spry::BasicMesh::load_data(std::span<float> vertices, std::span<int> format)
{
    if (m_data_loaded) {
        // std::cout << "Cannot reload data\n";
        // return;
        delete_buffers();
    }

    m_data_loaded = true;
    index_array = false;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), (void*)vertices.data(), GL_STATIC_DRAW);

    int stride = 0;
    for (int attribSize : format) {
        stride += attribSize;
    }
    
    m_vertex_count = vertices.size() / stride;

    long prevAttribSize = 0;
    for (int i = 0; i < format.size(); i++) {
        int attribSize = format[i];
        glVertexAttribPointer(i, attribSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(prevAttribSize * sizeof(float)));
        glEnableVertexAttribArray(i);
        prevAttribSize += attribSize;
    }
}

void spry::BasicMesh::draw(GLenum mode)
{
    glBindVertexArray(VAO);
    if (index_array) {
        glDrawElements(mode, m_vertex_count, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(mode, 0, m_vertex_count);
    }
}