#pragma once

#include <cmath>
#include <cstddef>
#include <type_traits>

#include "Vec2.h"

template <typename T>
    requires std::is_arithmetic_v<T>
class Mat2
{
public:
    using ValueType = T;

    constexpr Mat2() : elements{{T{1}, T{0}}, {T{0}, T{1}}} {}

    constexpr Mat2(T topLeft, T topRight, T bottomLeft, T bottomRight)
        : elements{{topLeft, topRight}, {bottomLeft, bottomRight}} {}

    constexpr Mat2& operator*=(T scalar)
    {
        for (auto& row : elements)
        {
            for (T& element : row)
            {
                element *= scalar;
            }
        }
        return *this;
    }

    [[nodiscard]] constexpr Mat2 operator*(T scalar) const
    {
        Mat2 result = *this;
        return result *= scalar;
    }

    [[nodiscard]] constexpr Mat2 operator*(const Mat2& other) const
    {
        Mat2 result;
        for (std::size_t row = 0; row < 2; ++row)
        {
            for (std::size_t column = 0; column < 2; ++column)
            {
                result.elements[row][column] =
                    elements[row][0] * other.elements[0][column] +
                    elements[row][1] * other.elements[1][column];
            }
        }
        return result;
    }

    constexpr Mat2& operator*=(const Mat2& other)
    {
        *this = *this * other;
        return *this;
    }

    [[nodiscard]] static constexpr Mat2 Identity()
    {
        return {T{1}, T{0}, T{0}, T{1}};
    }

    [[nodiscard]] static Mat2 Rotation(T angleRadians)
        requires std::is_floating_point_v<T>
    {
        const T cosine = static_cast<T>(std::cos(angleRadians));
        const T sine = static_cast<T>(std::sin(angleRadians));
        return {cosine, sine, -sine, cosine};
    }

    [[nodiscard]] static constexpr Mat2 Scaling(T factor)
    {
        return {factor, T{0}, T{0}, factor};
    }

    [[nodiscard]] constexpr bool operator==(const Mat2&) const = default;

public:
    T elements[2][2];
};

template <typename T>
    requires std::is_arithmetic_v<T>
[[nodiscard]] constexpr Vec2<T> operator*(const Vec2<T>& vector, const Mat2<T>& matrix)
{
    return {
        vector.x * matrix.elements[0][0] + vector.y * matrix.elements[1][0],
        vector.x * matrix.elements[0][1] + vector.y * matrix.elements[1][1]};
}

template <typename T>
    requires std::is_arithmetic_v<T>
constexpr Vec2<T>& operator*=(Vec2<T>& vector, const Mat2<T>& matrix)
{
    return vector = vector * matrix;
}

using Matrix2D = Mat2<float>;
using Matrix2Dd = Mat2<double>;
using Matrix2Di = Mat2<int>;
