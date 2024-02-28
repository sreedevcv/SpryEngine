#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "BasicMesh.hpp"

namespace spry {


class Mesh {
private:
	BasicMesh m_mesh;
	void setup_mesh();

public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coords;
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader &shader);
};

}