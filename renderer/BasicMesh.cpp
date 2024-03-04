#include "BasicMesh.hpp"

spry::BasicMesh::BasicMesh()
    : m_data_loaded(false)
{
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

// template<class T>
// void spry::BasicMesh::load_data(std::vector<T>& vertices, std::vector<unsigned int>& indices, std::vector<int>& format)
// {
//     if (m_data_loaded) {
//         // std::cout << "Cannot reload data\n";
//         // return;
//         delete_buffers();
//     }

//     m_vertex_count = indices.size();
//     m_data_loaded = true;
//     index_array = true;

//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);

//     glBindVertexArray(VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), (void*)vertices.data(), GL_STATIC_DRAW);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), (void*)indices.data(), GL_STATIC_DRAW);

//     int stride = 0;
//     for (int attribSize : format) {
//         stride += attribSize;
//     }

//     long prevAttribSize = 0;
//     for (int i = 0; i < format.size(); i++) {
//         int attribSize = format[i];
//         glVertexAttribPointer(i, attribSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(prevAttribSize * sizeof(float)));
//         glEnableVertexAttribArray(i);
//         prevAttribSize += attribSize;
//     }
// }

// template<class T>
// void spry::BasicMesh::load_data(std::vector<T>& vertices, std::vector<int>& format)
// {
//     if (m_data_loaded) {
//         // std::cout << "Cannot reload data\n";
//         // return;
//         delete_buffers();
//     }

//     m_data_loaded = true;
//     index_array = false;

//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);

//     glBindVertexArray(VAO);
//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), (void*)vertices.data(), GL_STATIC_DRAW);

//     int stride = 0;
//     for (int attribSize : format) {
//         stride += attribSize;
//     }
    
//     m_vertex_count = vertices.size() / stride;

//     long prevAttribSize = 0;
//     for (int i = 0; i < format.size(); i++) {
//         int attribSize = format[i];
//         glVertexAttribPointer(i, attribSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(prevAttribSize * sizeof(float)));
//         glEnableVertexAttribArray(i);
//         prevAttribSize += attribSize;
//     }
// }

void spry::BasicMesh::draw(GLenum mode)
{
    glBindVertexArray(VAO);
    if (index_array) {
        glDrawElements(mode, m_vertex_count, GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(mode, 0, m_vertex_count);
    }
    glBindVertexArray(0);
}

void spry::BasicMesh::operator=(BasicMesh& mesh)
{
    if (this != &mesh) {

    }
}

void spry::BasicMesh::operator=(BasicMesh&& mesh)
{
}

spry::BasicMesh::BasicMesh(BasicMesh& mesh)
{
    // if (this != &mesh) {
    //     this->VAO = mesh.VAO;
    //     this->EBO = mesh.EBO;
    //     this->VBO = mesh.VBO;
    //     this->m_vertex_count = mesh.m_vertex_count;
    // }
}

spry::BasicMesh::BasicMesh(BasicMesh&& mesh)
{
    if (this != &mesh) {
        this->VAO = mesh.VAO;
        this->EBO = mesh.EBO;
        this->VBO = mesh.VBO;
        this->m_vertex_count = mesh.m_vertex_count;
    }
}
