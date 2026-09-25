#pragma once

#include <vector>
#include <utility>
#include "math/Vector.h"

namespace geometry
{

    struct Mesh
    {
        Mesh() = delete;

        Mesh(std::vector<math::Vec3f> vertices, std::vector<unsigned> indices, bool isFormedOfTriangles)
        : vertices{std::move(vertices)}, indices{std::move(indices)}, isFormedOfTriangles(isFormedOfTriangles) {}

        std::vector<math::Vec3f> vertices;
        // Groups of three indices for triangles, pairs for line segments.
        std::vector<unsigned> indices;
        bool isFormedOfTriangles;
    };
}
