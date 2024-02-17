#pragma once

namespace spry {
template <class T = float>
class Vec2 {
public:
    T x;
    T y;

    Vec2()
    {
        x = 0;
        y = 0;
    }

    Vec2(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    ~Vec2() = default;

    Vec2& operator*(T& rhs)
    {
        x *= rhs;
        y *= rhs;
        return this;
    }
};

template <class T>
T dot(Vec2<T> a, Vec2<T> b)
{
    return a.x * b.x + a.y * b.y;
}

template <class T>
Vec2<T> operator+(const Vec2<T>& a, const Vec2<T>& b)
{
    auto result = a;
    result.x += a.x;
    result.y += a.y;
    return result;
}

template <class T>
Vec2<T> operator-(const Vec2<T>& a, const Vec2<T>& b)
{
    auto result = a;
    result.x -= a.x;
    result.y -= a.y;
    return result;
}
} // namespace spry
