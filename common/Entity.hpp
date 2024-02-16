#pragma once

#include "Vec2.hpp"

namespace spry {
	template<typename T=float>
	class Entity {
	protected:
		Vec2<T> mPosition;

	public:
		Entity(Vec2<T> position);
		~Entity() = default;

		void setPosition(Vec2<T>& position);
		Vec2<T>& getPosition();
	};
}