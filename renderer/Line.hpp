#pragma once

#include <glm/vec3.hpp>

#include "BasicMesh.hpp"

namespace spry {

class Line {
private:
    glm::vec3 m_start;
    glm::vec3 m_end;

    BasicMesh m_mesh;

    void load_mesh();

public:
    Line();
    Line(glm::vec3 start, glm::vec3 end );
    ~Line() = default;

    void set_end_points(glm::vec3 start, glm::vec3 end);
    void draw();
};

}