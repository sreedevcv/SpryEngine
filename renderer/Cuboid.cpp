#include "Cuboid.hpp"

spry::Cuboid::Cuboid(spry::Shader& shader) 
    : mShader(shader)
{
    initBuffers();
    mShader.compile();
    check_for_opengl_error();
}

void spry::Cuboid::initBuffers() {
    constexpr float vertices[] =  {
        // positions        // colours
        -0.5f, 0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 0.1f, // front top left
        0.5f, 0.5f, 0.5f,   1.0f, 1.0f, 1.0f, 0.1f, // front top right
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.1f, // front bottom left
        0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 0.1f, // front bottom right

        -0.5f, 0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.1f, // back top left
        0.5f, 0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 0.1f, // back top right
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.1f, // back bottom left
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.1f, // back bottom right
    };

    const unsigned int indices[] = {
        0, 3, 1, // front 1
        0, 2, 3, // front 2

        0, 4, 6, // left 1
        0, 6, 2, // left 2

        1, 7, 5, // right 1
        1, 3, 7, // right 2

        1, 4, 0, // top 1
        1, 5, 4, // top 2

        4, 5, 7, // back 1
        5, 6, 7, // back 2

        3, 6, 2, // bottom 1
        3, 7, 6, // bottom 2
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (void*)vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), (void*)indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

spry::Cuboid::~Cuboid() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
}

void spry::Cuboid::draw() {
    mShader.use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
