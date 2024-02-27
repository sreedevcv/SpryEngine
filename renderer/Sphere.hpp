#pragma once

#include "BasicMesh.hpp"

namespace spry {

class Sphere {
private:
    BasicMesh m_mesh;

public:
    Sphere() = default;
    ~Sphere() = default;

    void load(const float radius, int sectors, int stacks);

    void draw();
};

}