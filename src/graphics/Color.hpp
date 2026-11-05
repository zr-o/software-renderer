#pragma once

#include "math/Vector.hpp"

namespace graphics
{
using Color = math::Vector<float, 4>;

namespace Colors
{
inline constexpr graphics::Color Red{1.0f, 0.0f, 0.0f, 1.0f};
inline constexpr graphics::Color Green{0.0f, 1.0f, 0.0f, 1.0f};
inline constexpr graphics::Color Blue{0.0f, 0.0f, 1.0f, 1.0f};
inline constexpr graphics::Color Black{0.0f, 0.0f, 0.0f, 1.0f};
inline constexpr graphics::Color White{1.0f, 1.0f, 1.0f, 1.0f};
inline constexpr graphics::Color Orange{1.0f, 0.65f, 0.0f, 1.0f};
} // namespace Colors
} // namespace graphics
