#pragma once

#include <cmath>
#include <concepts>
#include <cstddef>
#include <type_traits>

namespace math
{

template <typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;

template <std::floating_point T, std::size_t Dimension>
    requires(Dimension > 0)
struct Vector
{
    using ValueType = T;
    static constexpr std::size_t Size = Dimension;

    T elements[Dimension]{};

    constexpr Vector() = default; // Initialized to 0 because of elements[]{};

    template <typename... Args>
        requires(sizeof...(Args) == Dimension)
    constexpr Vector(Args... args) : elements{static_cast<T>(args)...}
    {
    }

    template <std::floating_point U> // concept not needed here since U is always a floating_point if its a vector
    constexpr explicit Vector(const Vector<U, Dimension> &other)
    {
        for (std::size_t index = 0; index < Dimension; ++index)
        {
            elements[index] = static_cast<T>(other[index]);
        }
    }

    template <std::floating_point U, std::size_t OtherDimension>
    requires(Dimension < OtherDimension)
    constexpr explicit Vector(const Vector<U, OtherDimension> &other)
    {
        for (std::size_t index = 0; index < Dimension; ++index)
        {
            elements[index] = static_cast<T>(other[index]);
        }
    }

    constexpr T &operator[](std::size_t index) { return elements[index]; }
    constexpr const T &operator[](std::size_t index) const { return elements[index]; }

    [[nodiscard]] constexpr Vector operator-() const
    {
        Vector result;
        for (std::size_t index = 0; index < Dimension; ++index)
        {
            result[index] = -elements[index];
        }
        return result;
    }

    template <std::floating_point U>
    [[nodiscard]] constexpr Vector operator+(const Vector<U, Dimension> &other) const
    {
        Vector result;
        for (std::size_t index = 0; index < Dimension; ++index)
        {
            result[index] = static_cast<T>(elements[index] + other[index]);
        }
        return result;
    }

    template <std::floating_point U>
    [[nodiscard]] constexpr Vector operator-(const Vector<U, Dimension> &other) const
    {
        Vector result;
        for (std::size_t index = 0; index < Dimension; ++index)
        {
            result[index] = elements[index] - other[index];
        }
        return result;
    }

    template <std::floating_point U>
    constexpr Vector &operator+=(const Vector<U, Dimension> &other)
    {
        for (std::size_t index = 0; index < Dimension; ++index)
        {
            elements[index] += static_cast<T>(other[index]);
        }
        return *this;
    }

    template <std::floating_point U>
    constexpr Vector &operator-=(const Vector<U, Dimension> &other)
    {
        for (std::size_t index = 0; index < Dimension; ++index)
        {
            elements[index] -= static_cast<T>(other[index]);
        }
        return *this;
    }

    template <arithmetic Scalar>
    [[nodiscard]] constexpr Vector operator*(Scalar scalar) const
    {
        Vector result;
        for (std::size_t index = 0; index < Dimension; ++index)
        {
            result[index] = static_cast<T>(elements[index] * scalar);
        }
        return result;
    }

    template <arithmetic Scalar>
    [[nodiscard]] constexpr Vector operator/(Scalar scalar) const
    {
        Vector result;
        if (scalar == 0)
            return result;

        for (std::size_t index = 0; index < Dimension; ++index)
        {
            result[index] = static_cast<T>(elements[index] / scalar);
        }
        return result;
    }

    template <arithmetic Scalar>
    constexpr Vector &operator*=(Scalar scalar)
    {
        for (T &element : elements)
        {
            element = static_cast<T>(element * scalar);
        }
        return *this;
    }

    template <arithmetic Scalar>
    constexpr Vector &operator/=(Scalar scalar)
    {
        for (T &element : elements)
        {
            element = static_cast<T>(element / scalar);
        }
        return *this;
    }

    [[nodiscard]] constexpr bool operator==(const Vector &) const = default;

    template <std::floating_point U>
    [[nodiscard]] constexpr float dot(const Vector<U, Dimension> &other) const
    {
        float result{};
        for (std::size_t index = 0; index < Dimension; ++index)
        {
            result += static_cast<float>(elements[index] * other[index]);
        }
        return result;
    }

    [[nodiscard]] constexpr float lengthSquared() const { return dot(*this); }

    [[nodiscard]] constexpr float length() const
    {
        return std::sqrt(static_cast<float>(lengthSquared()));
    }

    [[nodiscard]] float distanceTo(const Vector &other) const
    {
        return (*this - other).length();
    }

    [[nodiscard]] Vector getNormalized() const
    {
        const T vectorLength = length();
        Vector result;
        if (vectorLength == 0.0f)
            return result;

        for (std::size_t index = 0; index < Dimension; ++index)
        {
            result[index] = static_cast<T>(elements[index]) / vectorLength;
        }
        return result;
    }

    constexpr Vector &normalize()
    {
        const T vectorLength = static_cast<T>(length());
        if (vectorLength == 0)
            return *this;

        for (T &element : elements)
        {
            element /= vectorLength;
        }
        return *this;
    }
};

template <arithmetic Scalar, typename T, std::size_t Dimension>
[[nodiscard]] constexpr Vector<T, Dimension> operator*(Scalar scalar, const Vector<T, Dimension> &vector)
{
    return vector * scalar;
}

using Vec2f = Vector<float, 2>;
using Vec3f = Vector<float, 3>;
}
