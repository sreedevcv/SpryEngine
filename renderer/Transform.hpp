#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace spry {

class Transform {
private:
    glm::mat4 model = glm::mat4(1.0f);

public:
    Transform& scale(const glm::vec3& scale);
    Transform& translate(const glm::vec3& location);
    Transform& rotate(float angle, const glm::vec3& axis);

    glm::mat4& get_model();
};

}