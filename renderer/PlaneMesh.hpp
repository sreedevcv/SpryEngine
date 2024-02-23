#pragma once

#include "BasicMesh.hpp"
#include "Shader.hpp"

#include <memory>

namespace spry
{

class PlaneMesh {
private:
    BasicMesh m_mesh;
    int m_length;
    int m_breadth;

    Shader m_shader;

public:
    PlaneMesh(float length, float breadth, int length_segments, int breadth_segments);

    void draw();
};

} // namespace spry
