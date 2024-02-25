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

public:
    PlaneMesh();
    PlaneMesh(float length, float breadth, int length_segments, int breadth_segments);

    void load(float length, float breadth, int length_segments, int breadth_segments);
    void draw();
};

} // namespace spry
