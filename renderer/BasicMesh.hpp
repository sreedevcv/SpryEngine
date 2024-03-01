#pragma once

// #include <span>
#include <vector>

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
    ~BasicMesh();

    template <class T>
    void load_data(std::vector<T>& vertices, std::vector<unsigned int>& indices, std::vector<int>& format)
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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), (void*)vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), (void*)indices.data(), GL_STATIC_DRAW);

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
        // glBindVertexArray(0);
    }

    template <class T>
    void load_data(std::vector<T>& vertices, std::vector<int>& format)
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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), (void*)vertices.data(), GL_STATIC_DRAW);

        int stride = 0;
        for (int attribSize : format) {
            stride += attribSize;
        }
        m_vertex_count = (vertices.size() * sizeof(float)) / stride;

        long prevAttribSize = 0;
        for (int i = 0; i < format.size(); i++) {
            int attribSize = format[i];
            glVertexAttribPointer(i, attribSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(prevAttribSize * sizeof(float)));
            glEnableVertexAttribArray(i);
            prevAttribSize += attribSize;
        }
        // glBindVertexArray(0);
    }
    
    void draw(GLenum mode = GL_TRIANGLES);

    void operator=(BasicMesh& mesh);
    void operator=(BasicMesh&& mesh);

    BasicMesh(BasicMesh& mesh);
    BasicMesh(BasicMesh&& mesh);
};

}
