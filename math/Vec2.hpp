#pragma once

namespace spry {
    template<class T=float>
    class Vec2 {
    public:
        T x;
        T y;

        Vec2(T x, T y);
        ~Vec2() = default;
    };

    template<class T>
    T dot(Vec2<T> a, Vec2<T> b);
}

