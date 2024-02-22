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
    }

    Mat4(float t)
        : Mat4()
    {
        if (static_cast<T>(t) == 1) {
            values[0] = 1.0;
            values[4 + 1] = 1.0;
            values[8 + 2] = 1.0;
            values[12 + 3] = 1.0;
        }
    }

    ~Mat4() = default;

    T* getData()
    {
        return values.data();
    }
};

template <class T>
Mat4<T> operator*(const Mat4<T>& lhs, const Mat4<T>& rhs)
{
    Mat4<T> result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            T sum = 0;
            for (int k = 0; k < 4; k++) {
                sum += lhs.values[i * 4 + k] * rhs.values[j * 4 + k];
            }
            result.values[i * 4 + j] = sum;
        }
    }

    return result;
}
}