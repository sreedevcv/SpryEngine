#include "Transform.hpp"

spry::Transform& spry::Transform::scale(const glm::vec3& scale)
{
    model = glm::scale(model, scale);
    return *this;
}

spry::Transform& spry::Transform::translate(const glm::vec3& location)
{
    model = glm::translate(model, location);
    return *this;
}

spry::Transform& spry::Transform::rotate(float angle, const glm::vec3& axis)
{
    model = glm::rotate(model, angle, axis);
    return *this;
}

glm::mat4& spry::Transform::get_model()
{
    return model;
}
