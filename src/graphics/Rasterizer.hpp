#pragma once

#include "graphics/Framebuffer.hpp"
#include "math/Vector.h"

namespace graphics
{

class Rasterizer
{
public:
    explicit Rasterizer(Framebuffer &frameBuffer) : frameBuffer_(frameBuffer) {}

    void drawLine(const math::Vec2f &p1, const math::Vec2f &p2, const Pixel &color);
    void drawTriangle(const math::Vec2f &p1, const math::Vec2f &p2, const math::Vec2f &p3, const Pixel &color);

private:
    Framebuffer &frameBuffer_;
};

}
