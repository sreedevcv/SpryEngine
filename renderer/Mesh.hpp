#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <cstddef>

#include "Shader.hpp"

// #define offsetof(s, m) sizeof((s)) + sizeof((m))
#define MAX_BONE_INFLUENCE 4

namespace spry {

class Mesh {
private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();

public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coords;
        glm::vec3 tangent;
        glm::vec3 bit_tangent;
        
        int boneIDs[MAX_BONE_INFLUENCE];
        int weights[MAX_BONE_INFLUENCE];
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;

    Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Texture> &textures);
    // ~Mesh();

    void draw(const Shader &shader);
};


}