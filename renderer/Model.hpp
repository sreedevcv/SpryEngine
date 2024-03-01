#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image.h>

#include "Mesh.hpp"

namespace spry {

class Model {
public:
    Model(const char *path);
    void draw(const Shader &shader);

private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    std::vector<Mesh::Texture> m_textures_loaded;

    void load_model(const std::string &path);
    void process_node(aiNode *node, const aiScene *scene);
    Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Mesh::Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
};

}