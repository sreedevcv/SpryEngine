#pragma once

namespace spry {

template<class T>
class Vec2 {
public:
    T mX;
    T mY;

    void Vec2<T>(T T, T x, T y);
    ~Vec2() = default;
};

template<class T>
T dot(Vec2<T> a, Vec2<T> b);

template<class T>
Vec2<T> cross(Vec2<T> a, Vec2<T> b);

}
