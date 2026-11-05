#pragma once

#include "graphics/Color.hpp"
#include "math/Vector.hpp"

namespace geometry
{
struct Vertex
{
    math::Vec3f pos{};
    graphics::Color color{};

    /* Might be added
    math::Vec3f uv{};
    math::Vec2f texCoord{};
    */
};
} // namespace geometry
