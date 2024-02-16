#pragma once

#include "Vec2.hpp"

namespace spry
{
	class Rect {
	public:
		Vec2<float> mPosition = { 0.0f, 0.0f };
		Vec2<float> mSize = { 0.0f, 0.0f };

		Rect(Vec2<float>& position, Vec2<float>& size);
		~Rect();
	};

}