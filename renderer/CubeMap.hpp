#pragma once

#include <vector>

#include "Shader.hpp"
#include "BasicMesh.hpp"

#include <glm/mat4x4.hpp>

namespace spry {

class CubeMap 
{
private:
    unsigned int m_texture_id;
    Shader m_shader;
    BasicMesh m_mesh;
    
public:
    CubeMap();
    void load_cube_map(const char* texture_paths[]);
    void draw(const glm::mat4& view, const glm::mat4& projection);
};

}