#pragma once

#include <vector>
#include <utility>
#include "geometry/Vertex.h"

namespace geometry
{

    struct Mesh
    {
        Mesh() = delete;

        Mesh(std::vector<geometry::Vertex> vertices, std::vector<unsigned> indices, bool isFormedOfTriangles)
        : vertices{std::move(vertices)}, indices{std::move(indices)}, isFormedOfTriangles(isFormedOfTriangles) {}

        std::vector<geometry::Vertex> vertices;
        // Groups of three indices for triangles, pairs for line segments.
        std::vector<unsigned> indices;
        bool isFormedOfTriangles;
    };
}
