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
        m_data_loaded = mesh.m_data_loaded;
        m_vertex_count = mesh.m_vertex_count;
        index_array = mesh.index_array;
        VAO = mesh.VAO;
        VBO = mesh.VBO;
        EBO = mesh.EBO;
    }
}

void spry::BasicMesh::operator=(BasicMesh&& mesh)
{
    if (this != &mesh) {
        m_data_loaded = mesh.m_data_loaded;
        m_vertex_count = mesh.m_vertex_count;
        index_array = mesh.index_array;
        VAO = mesh.VAO;
        VBO = mesh.VBO;
        EBO = mesh.EBO;
    }
}

spry::BasicMesh::BasicMesh(BasicMesh& mesh)
{
    if (this != &mesh) {
        m_data_loaded = mesh.m_data_loaded;
        m_vertex_count = mesh.m_vertex_count;
        index_array = mesh.index_array;
        VAO = mesh.VAO;
        VBO = mesh.VBO;
        EBO = mesh.EBO;
    }
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
