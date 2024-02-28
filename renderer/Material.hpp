#pragma once

#include <glm/glm.hpp>

namespace spry {

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

namespace material {

    Material obsidian = {
        .ambient = glm::vec3(0.05375, 0.05, 0.06625),
        .diffuse = glm::vec3(0.18275, 0.17, 0.22525),
        .specular = glm::vec3(0.332741, 0.328634, 0.346435),
        .shininess = 0.3 * 128
    };
}

}