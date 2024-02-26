#include "Entity.hpp"

spry::Entity::Entity(glm::vec2 position)
    : mPosition(position)
{
}

void spry::Entity::setPosition(glm::vec2& position)
{
    mPosition = position;
}

glm::vec2& spry::Entity::getPosition()
{
    return mPosition;
}
