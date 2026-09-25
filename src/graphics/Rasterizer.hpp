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

    // Screen coordinates: x increases rightward, y downward. The directed edge
    // must belong to a triangle with positive signed area (clockwise on screen).
    [[nodiscard]] static bool isTopLeftEdge(const math::Vec2f &start, const math::Vec2f &end);

    // Includes top/left boundaries, excludes bottom/right boundaries.
    // Accepts either winding; zero-area triangles contain no points.
    [[nodiscard]] static bool isInsideTriangle(const math::Vec2f &point,
                                             const math::Vec2f &p1,
                                             const math::Vec2f &p2,
                                             const math::Vec2f &p3);

private:
    Framebuffer &frameBuffer_;
};

}
