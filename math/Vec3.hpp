#pragma once 

namespace spry {
	template<class T=float>
	struct Vec3 {
		T x, y, z;

		Vec3()
			: x(0)
			, y(0)
			, z(0)
		{
		}

		Vec3(T x, T y, T z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vec3& operator*(T& rhs)
		{
			x *= rhs;
			y *= rhs;
			z *= rhs;
			return this;
		}
	};


template <class T>
T dot(Vec3<T> lhs, Vec3<T> rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template<class T>
Vec3<T> cross(Vec3<T> lhs, Vec3<T> rhs)
{
	Vec3<T> result = (
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.z
		);
	return result;
}

template <class T>
Vec3<T> operator+(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    auto result = lhs;
    result.x += rhs.x;
    result.y += rhs.y;
	result.z += rhs.z;
    return result;
}

template <class T>
Vec3<T> operator-(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    auto result = lhs;
    result.x -= rhs.x;
    result.y -= rhs.y;
	result.z -= rhs.z;
    return result;
}
}