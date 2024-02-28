#include "Transform.hpp"

void spry::Transform::set_scale(const glm::vec3& scale)
{
    model = glm::scale(model, scale);
}

void spry::Transform::set_translation(const glm::vec3& location)
{
    model = glm::translate(model, location);
}

void spry::Transform::set_rotation(float angle, const glm::vec3& axis)
{
    model = glm::rotate(model, angle, axis);
}

glm::mat4& spry::Transform::get_model()
{
    return model;
}
