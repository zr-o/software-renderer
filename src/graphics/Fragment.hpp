#pragma once

#include "graphics/Color.hpp"

namespace graphics
{

struct Fragment
{
    unsigned int x;
    unsigned int y;
    float depth;
    Color color;
};

} // namespace graphics
