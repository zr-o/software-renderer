#pragma once

#include "math/Vector.h"
#include "graphics/Pixel.hpp"

namespace geometry
{
    struct Vertex
    {
        math::Vec3f pos{};
        graphics::Pixel color{};

        /* Might be added
        math::Vec3f uv{};
        math::Vec2f texCoord{};
        */
    };
}