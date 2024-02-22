#include "BasicMesh.hpp"

spry::BasicMesh::BasicMesh(Shader& shader, std::span<float> vertices, std::span<int> indices, std::span<int> format)
    : mVertexCount(indices.size())
    , mShader(shader)
{
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

spry::BasicMesh::~BasicMesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
}

void spry::BasicMesh::draw()
{
    mShader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, mVertexCount, GL_UNSIGNED_INT, 0);
}
