#pragma once

#include <cmath>
#include <limits>

#include <Vec2.hpp>
#include <Vec3.hpp>
#include <Mat4.hpp>

namespace spry {

template <class T>
Vec2<T> norm(const Vec2<T>&& v)
{
    Vec2<T> normVec = v;
    T root = static_cast<T>(std::sqrt(v.x * v.x + v.y * v.y));

    if (root < std::numeric_limits<T>::epsilon()) {
        Vec2<T> nv = v * static_cast<T>(100000);
        return norm(nv);
    }

    normVec.x /= root;
    normVec.y /= root;
    return normVec;
}

template <class T>
Vec3<T> norm(const Vec3<T>& v)
{
    Vec3<T> normVec = v;
    T root = (magnitude(v));

    // if (root < std::numeric_limits<T>::epsilon()) {
    //     return Vec3<T>(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));
    // }

    normVec.x /= root;
    normVec.y /= root;
    normVec.z /= root;

    return normVec;
}

template <class T>
Mat4<T> look_at(const Vec3<T>& eye, const Vec3<T>& target, const Vec3<T>& up)
{
    const auto forward = norm(target - eye);
    const auto right = norm(cross(forward, up));
    const auto newUp = norm(cross(right, forward));
    const auto tX = dot(right, eye);
    const auto tY = dot(newUp, eye);
    const auto tZ = dot(forward, eye);

    Mat4<T> m(1);
    m.values[0] = right.x;
    m.values[1] = right.y;
    m.values[2] = right.z;
    m.values[3] = 0;

    m.values[4] = newUp.x;
    m.values[5] = newUp.y;
    m.values[6] = newUp.z;
    m.values[7] = 0;

    m.values[8] = -forward.x;
    m.values[9] = -forward.y;
    m.values[10] = -forward.z;
    m.values[11] = 0;

    m.values[12] = -tX;
    m.values[13] = -tY;
    m.values[14] = tZ;
    m.values[15] = 1;

    return m;
}

// template <class T>
inline Mat4<float> perspective_projection(const float nearPoint, const float farPoint, const float aspectRatio, const float fov)
{
    // assert(abs(aspectRatio - std::numeric_limits<float>::epsilon()) > static_cast<float>(0));

    Mat4<float> p;
    const float tanHalfFov = std::tan(fov / 2.0f);
    const float depth = farPoint - nearPoint;

    p.values[0 * 4 + 0] = 1.0f / (aspectRatio * tanHalfFov);
    p.values[1 * 4 + 1] = 1.0f / tanHalfFov;
    p.values[2 * 4 + 2] = -(farPoint + nearPoint) / depth;
    p.values[2 * 4 + 3] = -1.0f;
    p.values[3 * 4 + 2] = -(2.0f * farPoint * nearPoint) / depth;
    return p;
}

template <class T>
Vec3<T> operator*(const Mat4<T>& m, const Vec3<T>& v)
{
    // T values[4] = { v.values[0], v.values[1], v.values[2], 0 };
    // T result[4] = { 0 };

    // for (int i = 0; i < 4; i++) {
    // 	T result[i] = 0;
    // 	for (int j = 0; j < 4; j++) {
    // 		result[i] += m.values[i][j] * values[j];
    // 	}
    // }
    // return Vec3<T>(result[0], result[1], result[2]);

    T values[4] = { v.x, v.y, v.z, 1.0 }; // Include the homogeneous coordinate
    T result[4] = { 0 };

    for (int i = 0; i < 4; i++) {
        result[i] = 0;
        for (int j = 0; j < 4; j++) {
            result[i] += m.values[i * 4 + j] * values[j];
        }
    }

    // Divide by the homogeneous coordinate to project back to 3D space
    T invHomogeneous = 1.0 / result[3];
    return Vec3<T>(result[0] * invHomogeneous, result[1] * invHomogeneous, result[2] * invHomogeneous);
}

template <class T>
T radians(T degrees)
{
    return (degrees * static_cast<T>(3.14159)) / static_cast<T>(180);
}

template<class T>
T magnitude(Vec2<T> &v)
{
    return static_cast<T>(std::sqrt(dot(v, v)));
}

template<class T>
T magnitude(const Vec3<T> &v)
{
    return static_cast<T>(std::sqrt(dot(v, v)));
}

}