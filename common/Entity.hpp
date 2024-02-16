#pragma once

#include "Vec2.hpp"

namespace spry {
	template<typename T=float>
	class Entity {
	protected:
		Vec2<T> mPosition;

	public:
		Entity(Vec2<T> position)
			: mPosition(position)
		{}

		virtual ~Entity() = default;

		void setPosition(Vec2<T>& position)
		{
			mPosition = position;
		}

		Vec2<T>& getPosition()
		{
			return mPosition;
		}

		virtual void draw() = 0;
		virtual void update(float deltaTime) = 0;
	};
}