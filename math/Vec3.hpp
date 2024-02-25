#pragma once

#include <iostream>

namespace spry {
template <class T>
struct Vec3 {
    T x, y, z;

    Vec3()
        : x(0)
        , y(0)
        , z(0)
    {
    }

    Vec3(T x_, T y_, T z_)
        : x(x_)
        , y(y_)
        , z(z_)
    {
    }

    Vec3& operator*=(const T& rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    Vec3& operator+=(const Vec3<T>& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vec3& operator-=(const Vec3<T>& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }
};

template <class T>
T dot(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

template <class T>
Vec3<T> cross(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    Vec3<T> result(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x);
    return result;
}

template <class T>
Vec3<T> operator+(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    Vec3<T> result = lhs;
    result.x += rhs.x;
    result.y += rhs.y;
    result.z += rhs.z;
    return result;
}

template <class T>
Vec3<T> operator-(const Vec3<T>& lhs, const Vec3<T>& rhs)
{
    Vec3<T> result = lhs;
    result.x -= rhs.x;
    result.y -= rhs.y;
    result.z -= rhs.z;
    return result;
}

template <class T>
Vec3<T> operator*(const Vec3<T>& lhs, const T rhs)
{
    Vec3<T> result = lhs;
    result.x *= rhs;
    result.y *= rhs;
    result.z *= rhs;
    return result;
}

}