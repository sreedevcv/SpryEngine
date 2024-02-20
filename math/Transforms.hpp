#pragma once

#include <cmath>

#include <Vec2.hpp>
#include <Vec3.hpp>
#include <Mat4.hpp>

namespace spry {

template<class T>
Vec2<T> norm(Vec2<T>& v)
{
	return static_cast<T>(std::sqrt(v.x * v.x + v.y * v.y));
}

template<class T>
Vec3<T> norm(Vec3<T>& v)
{
	return static_cast<T>(std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

template<class T>
Mat4<T> lookAt()
{

}

// most likely wrong
template<class T>
Vec3<T> operator*(Mat4<T>& m, Vec3<T>& v)
{
	Vec3<T> result;

	for (int i = 0; i < 4; i++) {
		T sum = 0;
		for (int j = 0; j < 3; j++) {
			sum += m.values[i][j] * v[j]
		}
		sum += m.values[i][3];
		result[i] = sum;
	}
}

}