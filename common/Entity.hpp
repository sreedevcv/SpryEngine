#pragma once

#include <glm/vec2.hpp>

namespace spry {
	class Entity {
	protected:
		glm::vec2 mPosition;

	public:
		Entity(glm::vec2 position);

		virtual ~Entity() = default;

		void setPosition(glm::vec2& position);

		glm::vec2& getPosition();

		virtual void draw() = 0;
		virtual void update(float deltaTime) = 0;
	};
}