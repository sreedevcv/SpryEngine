#pragma once

#include <vector>

#include <glm/vec3.hpp>

#include "BasicMesh.hpp"

namespace spry {

class Point {
private:
    BasicMesh m_mesh;

public:
    Point();
    Point(std::vector<glm::vec3>& points);
    ~Point() = default;

    void load(std::vector<glm::vec3>& points);
    void draw();
    void set_point_size(float size);
};

}