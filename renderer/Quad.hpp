#pragma once 

#include "BasicMesh.hpp"

namespace spry
{

class Quad
{
private:
    spry::BasicMesh m_mesh;

public:
    Quad();
    ~Quad() = default;

    void draw();
};




}
