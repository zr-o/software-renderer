#pragma once

#include <cmath>
#include <cstddef>
#include <type_traits>

template <typename T>
    requires std::is_arithmetic_v<T>
struct Vec2
{
    using ValueType = T;

    T x{};
    T y{};

    constexpr Vec2() = default;
    constexpr Vec2(T xValue, T yValue) : x(xValue), y(yValue) {}

    template <typename U>
        requires std::is_convertible_v<U, T>
    constexpr explicit Vec2(const Vec2<U>& other)
        : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

    [[nodiscard]] constexpr Vec2 operator-() const { return {-x, -y}; }

    [[nodiscard]] constexpr Vec2 operator+(const Vec2& other) const
    {
        return {x + other.x, y + other.y};
    }

    [[nodiscard]] constexpr Vec2 operator-(const Vec2& other) const
    {
        return {x - other.x, y - other.y};
    }

    constexpr Vec2& operator+=(const Vec2& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr Vec2& operator-=(const Vec2& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    template <typename Scalar>
        requires std::is_arithmetic_v<Scalar>
    [[nodiscard]] constexpr Vec2<std::common_type_t<T, Scalar>> operator*(Scalar scalar) const
    {
        using ResultType = std::common_type_t<T, Scalar>;
        return {static_cast<ResultType>(x) * scalar, static_cast<ResultType>(y) * scalar};
    }

    template <typename Scalar>
        requires std::is_arithmetic_v<Scalar>
    [[nodiscard]] constexpr Vec2<std::common_type_t<T, Scalar>> operator/(Scalar scalar) const
    {
        using ResultType = std::common_type_t<T, Scalar>;

        if (scalar == 0) return {};
        
        return {static_cast<ResultType>(x) / scalar, static_cast<ResultType>(y) / scalar};
    }

    template <typename Scalar>
        requires std::is_arithmetic_v<Scalar>
    constexpr Vec2& operator*=(Scalar scalar)
    {
        x = static_cast<T>(x * scalar);
        y = static_cast<T>(y * scalar);
        return *this;
    }

    template <typename Scalar>
        requires std::is_arithmetic_v<Scalar>
    constexpr Vec2& operator/=(Scalar scalar)
    {
        x = static_cast<T>(x / scalar);
        y = static_cast<T>(y / scalar);
        return *this;
    }

    [[nodiscard]] constexpr bool operator==(const Vec2&) const = default;

    [[nodiscard]] constexpr T dot(const Vec2& other) const
    {
        return x * other.x + y * other.y;
    }

    [[nodiscard]] constexpr T lengthSquared() const
    {
        return dot(*this);
    }

    [[nodiscard]] float length() const
    {
        return std::sqrt(static_cast<float>(lengthSquared()));
    }

    [[nodiscard]] float distanceTo(const Vec2& other) const
    {
        return (*this - other).length();
    }

    [[nodiscard]] Vec2<float> getNormalized() const
    {
        const float vectorLength = length();
        if (vectorLength == 0.0f)
        {
            return {};
        }

        return {static_cast<float>(x) / vectorLength,
                static_cast<float>(y) / vectorLength};
    }

    Vec2& normalize() requires std::is_floating_point_v<T>
    {
        const T vectorLength = static_cast<T>(length());
        if (vectorLength == 0)
        {
            return *this;
        }

        x /= vectorLength;
        y /= vectorLength;
        return *this;
    }
};

template <typename Scalar, typename T>
    requires std::is_arithmetic_v<Scalar> && std::is_arithmetic_v<T>
[[nodiscard]] constexpr Vec2<std::common_type_t<T, Scalar>> operator*(Scalar scalar,
                                                                        const Vec2<T>& vector)
{
    return vector * scalar;
}

using Vector2D = Vec2<float>;
using Vector2Dd = Vec2<double>;
using Vector2Di = Vec2<int>;
