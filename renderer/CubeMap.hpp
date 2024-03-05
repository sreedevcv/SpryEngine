#pragma once

#include <vector>

#include "Shader.hpp"
#include "BasicMesh.hpp"

#include <glm/mat4x4.hpp>

/*
The depth func should be GL_EQUAL to qork properly.
The images hould not be flipped vertlically to properly
render the skybox.
*/

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
    void bind();
};

}