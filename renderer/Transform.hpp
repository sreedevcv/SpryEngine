#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace spry {

class Transform {
private:
    glm::mat4 model = glm::mat4(1.0f);

public:
    void set_scale(const glm::vec3& scale);
    void set_translation(const glm::vec3& location);
    void set_rotation(float angle, const glm::vec3& axis);

    glm::mat4& get_model();
};

}