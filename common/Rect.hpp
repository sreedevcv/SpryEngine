#pragma once

#include <glm/vec2.hpp>

namespace spry
{
	class Rect {
	public:
		glm::vec2 mPosition = { 0.0f, 0.0f };
		glm::vec2 mSize = { 0.0f, 0.0f };

		Rect(glm::vec2& position, glm::vec2& size);
		~Rect();
	};

}