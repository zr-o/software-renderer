#pragma once

#include "Mesh.hpp"

namespace geometry
{
    namespace primitives
    {
        Mesh createCube(float s)
        {
            return {{
                        {-s, -s, -s}, // 0
                        {s, -s, -s},  // 1
                        {-s, s, -s},  // 2
                        {s, s, -s},   // 3

                        {-s, -s, s}, // 4
                        {s, -s, s},  // 5
                        {-s, s, s},  // 6
                        {s, s, s}    // 7
                    },
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
                     3u, 7u}};
        }
    }
}