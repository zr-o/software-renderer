#pragma once

#include "Mesh.hpp"

namespace geometry
{
    namespace primitives
    {
        // s is the half-extent; triangle faces have outward-facing winding.
        inline Mesh createCube(float s, bool isFormedOfTriangles = true)
        {
            std::vector<math::Vec3f> vertices{
                        {-s, -s, -s}, // 0
                        {s, -s, -s},  // 1
                        {-s, s, -s},  // 2
                        {s, s, -s},   // 3

                        {-s, -s, s}, // 4
                        {s, -s, s},  // 5
                        {-s, s, s},  // 6
                        {s, s, s}    // 7
                    };

            if (isFormedOfTriangles)
            {
                return {std::move(vertices),
                        {
                            0u, 2u, 3u, 0u, 3u, 1u, // Back (-Z)
                            4u, 5u, 7u, 4u, 7u, 6u, // Front (+Z)
                            0u, 4u, 6u, 0u, 6u, 2u, // Left (-X)
                            1u, 3u, 7u, 1u, 7u, 5u, // Right (+X)
                            0u, 1u, 5u, 0u, 5u, 4u, // Bottom (-Y)
                            2u, 6u, 7u, 2u, 7u, 3u  // Top (+Y)
                        },
                        true};
            }

            return {std::move(vertices),
                    {// Back face
                     0u, 1u,
                     1u, 3u,
                     3u, 2u,
                     2u, 0u,

                     // Front face
                     4u, 5u,
                     5u, 7u,
                     7u, 6u,
                     6u, 4u,

                     // Connect front to back
                     0u, 4u,
                     1u, 5u,
                     2u, 6u,
                     3u, 7u},
                    false};
        }
    }
}
