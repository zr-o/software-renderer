#include "graphics/Rasterizer.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace
{
    double edgeFunction(const math::Vec2f &start, const math::Vec2f &end, const math::Vec2f &point)
    {
        // Promote before subtraction to keep intermediate calculations in double.
        return (static_cast<double>(end.x()) - start.x()) * (static_cast<double>(point.y()) - start.y()) - (static_cast<double>(end.y()) - start.y()) * (static_cast<double>(point.x()) - start.x());
    }
}

bool graphics::Rasterizer::isTopLeftEdge(const math::Vec2f &start, const math::Vec2f &end)
{
    return end.y() < start.y() || (end.y() == start.y() && end.x() > start.x());
}

bool graphics::Rasterizer::isInsideTriangle(const math::Vec2f &point,
                                            const math::Vec2f &p1,
                                            const math::Vec2f &p2,
                                            const math::Vec2f &p3)
{
    const double area = edgeFunction(p1, p2, p3);
    if (area == 0.0)
    {
        return false;
    }

    // Reverse negative-area triangles so the interior lies on the positive
    // side of every edge and the boundary rule is independent of winding.
    const math::Vec2f &b = area > 0.0 ? p2 : p3;
    const math::Vec2f &c = area > 0.0 ? p3 : p2;
    const auto acceptsEdge = [&point](const math::Vec2f &start, const math::Vec2f &end)
    {
        const double edge = edgeFunction(start, end, point);
        return edge > 0.0 || (edge == 0.0 && isTopLeftEdge(start, end));
    };

    return acceptsEdge(p1, b) && acceptsEdge(b, c) && acceptsEdge(c, p1);
}

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
    const double area = edgeFunction(p1, p2, p3);
    if (!std::isfinite(area) || area == 0.0)
        return;

    const math::Vec2f &b = area > 0.0 ? p2 : p3;
    const math::Vec2f &c = area > 0.0 ? p3 : p2;

    // Bound pixel centers (x + 0.5, y + 0.5), clipping before converting
    // to unsigned so off-screen triangles cannot cause invalid writes.
    const double width = frameBuffer_.getWidth();
    const double height = frameBuffer_.getHeight();
    const unsigned xBegin = static_cast<unsigned>(std::clamp(std::ceil(
        static_cast<double>(std::min({p1.x(), b.x(), c.x()})) - 0.5), 0.0, width));
    const unsigned xEnd = static_cast<unsigned>(std::clamp(std::floor(
        static_cast<double>(std::max({p1.x(), b.x(), c.x()})) - 0.5) + 1.0, 0.0, width));
    const unsigned yBegin = static_cast<unsigned>(std::clamp(std::ceil(
        static_cast<double>(std::min({p1.y(), b.y(), c.y()})) - 0.5), 0.0, height));
    const unsigned yEnd = static_cast<unsigned>(std::clamp(std::floor(
        static_cast<double>(std::max({p1.y(), b.y(), c.y()})) - 0.5) + 1.0, 0.0, height));
    if (xBegin >= xEnd || yBegin >= yEnd)
        return;

    const bool include0 = isTopLeftEdge(p1, b);
    const bool include1 = isTopLeftEdge(b, c);
    const bool include2 = isTopLeftEdge(c, p1);
    // Moving a sample one pixel right changes its edge value by -dy.
    const double step0 = static_cast<double>(p1.y()) - b.y();
    const double step1 = static_cast<double>(b.y()) - c.y();
    const double step2 = static_cast<double>(c.y()) - p1.y();
    Pixel *pixels = frameBuffer_.getRawPixelData();

    for (unsigned y = yBegin; y < yEnd; ++y)
    {
        // Recompute each row's starting values to limit accumulated error.
        const math::Vec2f start{xBegin + 0.5f, y + 0.5f};
        double e0 = edgeFunction(p1, b, start);
        double e1 = edgeFunction(b, c, start);
        double e2 = edgeFunction(c, p1, start);
        Pixel *pixel = pixels + static_cast<std::size_t>(y) * frameBuffer_.getWidth() + xBegin;
        for (unsigned x = xBegin; x < xEnd; ++x, ++pixel)
        {
            if ((e0 > 0.0 || (e0 == 0.0 && include0)) &&
                (e1 > 0.0 || (e1 == 0.0 && include1)) &&
                (e2 > 0.0 || (e2 == 0.0 && include2)))
            {
                *pixel = color;
            }
            e0 += step0;
            e1 += step1;
            e2 += step2;
        }
    }
}
