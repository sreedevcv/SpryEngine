#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "Mesh.hpp"

namespace spry {

class Model {
private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    std::vector<Mesh::Texture> textures_loaded; 

    void load_model(std::string path);
    void process_node(aiNode *node, const aiScene* scene);
    Mesh process_mesh(aiMesh *mesh, const aiScene* scene);
    std::vector<Mesh::Texture> load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name);

public:
    Model(char *path);
    void draw(Shader& shader);
};

}