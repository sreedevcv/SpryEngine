#pragma once

#include <array>
#include <cstring>

namespace spry {
template <class T = float>
class Mat4 {
public:
    std::array<T, 16> values;

    Mat4()
    {
        memset(values.data(), 0, sizeof(values));
        values[0] = 1.0;
        values[4 + 1] = 1.0;
        values[8 + 2] = 1.0;
        values[12 + 3] = 1.0;
    }

    ~Mat4() = default;

    T* getData()
    {
        return values.data();
    }
};
};