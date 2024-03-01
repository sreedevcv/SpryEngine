#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "BasicMesh.hpp"

    #define MAX_BONE_INFLUENCE 4


namespace spry {


class Mesh {
private:
	BasicMesh m_mesh;
	void setup_mesh();

    unsigned int VAO, VBO, EBO;

public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coords;
        glm::vec3 tangent;
        glm::vec3 bitangent;
        	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	void draw(Shader &shader);
};

}