#pragma once

#include <vector>
#include "math/Vector.h"

namespace geometry
{

    struct Mesh
    {
        Mesh() = delete;

        Mesh(std::vector<math::Vec3f> vertices, std::vector<unsigned> indices) 
        : vertices{std::move(vertices)}, indices{std::move(indices)} {}

        std::vector<math::Vec3f> vertices;
        std::vector<unsigned> indices;
    };
}