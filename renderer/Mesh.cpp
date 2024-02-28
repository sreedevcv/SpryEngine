#include "Mesh.hpp"

#include <span>

spry::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    : m_vertices(vertices)
    , m_indices(indices)
    , m_textures(textures)
{
    setup_mesh();
}

void spry::Mesh::setup_mesh()
{
    m_vertices.shrink_to_fit();
    m_indices.shrink_to_fit();
    m_textures.shrink_to_fit();

    int format[] = { 3, 3, 2 };

    // A chance for overflow since m_indices are converted to int from uint
    m_mesh.load_data(
        std::span<float> { (float*)m_vertices.data(), m_vertices.size() * 8 },
        std::span<int> { (int*)m_indices.data(), m_indices.size() },
        std::span<int> { format });
}

void spry::Mesh::draw(Shader& shader)
{
    unsigned int diffuse_count = 1;
    unsigned int specular_count = 1;

    for (int i = 0; i < m_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string& name = m_textures[i].type;

        if (name == "texture_diffuse") {
            number = std::to_string(diffuse_count);
            diffuse_count += 1;
        } else if (name == "texture_specular") {
            number = std::to_string(specular_count);
            specular_count += 1;
        }

        shader.set_uniform_int(("material" + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);
    m_mesh.draw();
}