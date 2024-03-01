#include "Mesh.hpp"

// #include <span>
// #include <algorithm>

spry::Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
    : m_vertices(vertices)
    , m_indices(indices)
    , m_textures(textures)
{
    setup_mesh();
}

void spry::Mesh::setup_mesh()
{
    // std::vector<int> format = { 3, 3, 2 };
    // m_mesh.load_data(m_vertices, m_indices, format);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
            // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(8 * sizeof(float)));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(11 * sizeof(float)));
		// ids
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)(15 * sizeof(float)));

		// weights
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(20 * sizeof(float)));
        glBindVertexArray(0);
}

void spry::Mesh::draw(Shader& shader)
{
    unsigned int diffuse_count = 1;
    unsigned int specular_count = 1;
    unsigned int normal_count = 1;
    unsigned int height_count = 1;

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
        } else if (name == "texture_normal") {
            number = std::to_string(normal_count);
            normal_count += 1;
        } else if (name == "texture_height") {
            number = std::to_string(height_count);
            height_count += 1;
        }

        // std::cout << (name + number) << "\n";
        shader.set_uniform_int((name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }

    // m_mesh.draw();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
    check_for_opengl_error();
}