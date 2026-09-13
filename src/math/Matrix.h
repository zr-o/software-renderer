#pragma once

#include <cmath>
#include <cstddef>
#include <concepts>

#include "Vector.h"

template <std::floating_point T, std::size_t Rows, std::size_t Columns>
struct Matrix
{
    using ValueType = T;

    constexpr Matrix() = default;

    template <typename... Args>
        requires(sizeof...(Args) == Rows * Columns)
    constexpr Matrix(Args... args) : elements{static_cast<T>(args)...}
    {
    }

    template <std::floating_point U>
    constexpr explicit Matrix(const Matrix<U, Rows, Columns> &other)
    {
        for (std::size_t row = 0; row < Rows; ++row)
        {
            for (std::size_t column = 0; column < Columns; ++column)
            {
                elements[row][column] = static_cast<T>(other[row][column]);
            }
        }
    }

    template <std::floating_point U>
    constexpr Matrix &operator=(const Matrix<U, Rows, Columns> &other)
    {
        for (std::size_t row = 0; row < Rows; ++row)
        {
            for (std::size_t column = 0; column < Columns; ++column)
            {
                elements[row][column] = static_cast<T>(other[row][column]);
            }
        }
        return *this;
    }

    constexpr T *operator[](std::size_t row) { return elements[row]; }
    constexpr const T *operator[](std::size_t row) const { return elements[row]; }

    template <arithmetic Scalar>
    constexpr Matrix &operator*=(Scalar scalar)
    {
        for (auto &row : elements)
        {
            for (T &element : row)
            {
                element *= scalar;
            }
        }
        return *this;
    }

    template <arithmetic Scalar>
    [[nodiscard]] constexpr Matrix operator*(Scalar scalar) const
    {
        Matrix result = *this;
        return result *= scalar;
    }

    template <std::floating_point U, std::size_t OtherColumns>
    [[nodiscard]] constexpr Matrix<T, Rows, OtherColumns> operator*(
        const Matrix<U, Columns, OtherColumns> &other) const
    {
        Matrix<T, Rows, OtherColumns> result;

        for (std::size_t row = 0; row < Rows; ++row)
        {
            for (std::size_t column = 0; column < OtherColumns; ++column)
            {
                for (std::size_t k = 0; k < Columns; ++k)
                {
                    result.elements[row][column] +=
                        elements[row][k] * static_cast<T>(other.elements[k][column]);
                }
            }
        }
        return result;
    }

    constexpr Matrix &operator*=(const Matrix &other)
        requires(Rows == Columns)
    {
        *this = *this * other;
        return *this;
    }

    [[nodiscard]] static constexpr Matrix Identity()
        requires(Rows == Columns)
    {
        Matrix identityMatrix;

        for (std::size_t row = 0; row < Rows; row++)
        {
            for (std::size_t column = 0; column < Columns; column++)
            {
                if (row == column)
                    identityMatrix.elements[row][column] = T{1};
            }
        }

        return identityMatrix;
    }

    template <std::size_t FirstAxis, std::size_t SecondAxis>
    [[nodiscard]] static Matrix Rotation(T angleRadians)
        requires(Rows == Columns &&
                 FirstAxis < Rows && SecondAxis < Rows && FirstAxis != SecondAxis)
    {
        Matrix rotation = Identity();
        const T cosine = static_cast<T>(std::cos(angleRadians));
        const T sine = static_cast<T>(std::sin(angleRadians));

        rotation[FirstAxis][FirstAxis] = cosine;
        rotation[FirstAxis][SecondAxis] = sine;
        rotation[SecondAxis][FirstAxis] = -sine;
        rotation[SecondAxis][SecondAxis] = cosine;

        return rotation;
    }

    [[nodiscard]] static Matrix Rotation(T angleRadians)
        requires(Rows == 2 && Columns == 2)
    {
        return Rotation<0, 1>(angleRadians);
    }

    [[nodiscard]] static constexpr Matrix Scaling(T factor)
        requires(Rows == Columns)
    {
        Matrix scaling;
        for (std::size_t index = 0; index < Rows; ++index)
        {
            scaling[index][index] = factor;
        }
        return scaling;
    }

    [[nodiscard]] constexpr bool operator==(const Matrix &) const = default;

    T elements[Rows][Columns]{};
};

template <typename T, std::size_t Dimension>
[[nodiscard]] constexpr Vector<T, Dimension> operator*(
    const Vector<T, Dimension> &vector,
    const Matrix<T, Dimension, Dimension> &matrix)
{
    Vector<T, Dimension> result;
    for (std::size_t column = 0; column < Dimension; ++column)
    {
        for (std::size_t row = 0; row < Dimension; ++row)
        {
            result[column] += vector[row] * matrix.elements[row][column];
        }
    }
    return result;
}

template <typename T, std::size_t Dimension>
constexpr Vector<T, Dimension> &operator*=(
    Vector<T, Dimension> &vector,
    const Matrix<T, Dimension, Dimension> &matrix)
{
    return vector = vector * matrix;
}

using Matrix2D = Matrix<float, 2, 2>;
using Matrix2Dd = Matrix<double, 2, 2>;
