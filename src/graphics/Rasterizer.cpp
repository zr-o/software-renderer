#include "graphics/Rasterizer.hpp"

#include <cmath>
#include <cstdint>

void graphics::Rasterizer::drawLine(const math::Vec2f &p1, const math::Vec2f &p2, const Pixel &color)
{
    const unsigned int x0 = static_cast<unsigned int>(std::round(p1[0]));
    const unsigned int y0 = static_cast<unsigned int>(std::round(p1[1]));
    const unsigned int x1 = static_cast<unsigned int>(std::round(p2[0]));
    const unsigned int y1 = static_cast<unsigned int>(std::round(p2[1]));

    // Signed intermediates keep decreasing coordinates and the error term safe.
    std::int64_t x = x0;
    std::int64_t y = y0;
    const std::int64_t dx = std::abs(static_cast<std::int64_t>(x1) - x);
    const std::int64_t dy = -std::abs(static_cast<std::int64_t>(y1) - y);
    const int stepX = x0 < x1 ? 1 : -1;
    const int stepY = y0 < y1 ? 1 : -1;
    std::int64_t error = dx + dy;

    while (true)
    {
        frameBuffer_.putPixel(static_cast<unsigned int>(x), static_cast<unsigned int>(y), color);
        if (x == x1 && y == y1)
        {
            break;
        }

        const std::int64_t doubledError = 2 * error;
        if (doubledError >= dy)
        {
            error += dy;
            x += stepX;
        }
        if (doubledError <= dx)
        {
            error += dx;
            y += stepY;
        }
    }
}

void graphics::Rasterizer::drawTriangle(const math::Vec2f &p1, const math::Vec2f &p2, const math::Vec2f &p3, const Pixel &color)
{
}
