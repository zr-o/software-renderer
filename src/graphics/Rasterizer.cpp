#include "graphics/Rasterizer.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

float graphics::Rasterizer::edgeFunction(const math::Vec2f &start, const math::Vec2f &end, const math::Vec2f &point)
{
    return ((end.x() - start.x()) * (point.y() - start.y()) - (end.y() - start.y()) * (point.x() - start.x()));
}

// This assumes that the triangle follows CCW vertex orientation
// How ever this is calculated like its CW vertex rotation because on the canvas its inversed
bool graphics::Rasterizer::isTopLeftEdge(const math::Vec2f &start, const math::Vec2f &end)
{
    return end.y() < start.y() || (end.y() == start.y() && end.x() > start.x());
}

bool graphics::Rasterizer::isInsideTriangle(const math::Vec2f &point,
                                            const math::Vec2f &p1,
                                            const math::Vec2f &p2,
                                            const math::Vec2f &p3)
{
    constexpr float tolerancePixels = 1e-5f;

    auto passesEdge = [&](const math::Vec2f &a, const math::Vec2f &b)
    {
        const float edge = edgeFunction(a, b, point);

        const float dx = b.x() - a.x();
        const float dy = b.y() - a.y();

        const float edgeLength = std::sqrt(dx * dx + dy * dy);
        const float epsilon = tolerancePixels * edgeLength;

        return edge > epsilon ||
               (std::abs(edge) <= epsilon && isTopLeftEdge(a, b));
    };

    return passesEdge(p1, p2) &&
           passesEdge(p2, p3) &&
           passesEdge(p3, p1);
}

void graphics::Rasterizer::drawLine(const geometry::Vertex &p1, const geometry::Vertex &p2, const Pixel &color)
{
    const unsigned int x0 = static_cast<unsigned int>(std::round(p1.pos.x()));
    const unsigned int y0 = static_cast<unsigned int>(std::round(p1.pos.y()));
    const unsigned int x1 = static_cast<unsigned int>(std::round(p2.pos.x()));
    const unsigned int y1 = static_cast<unsigned int>(std::round(p2.pos.y()));

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

void graphics::Rasterizer::drawTriangle(const geometry::Vertex &p1, const geometry::Vertex &p2, const geometry::Vertex &p3)
{
    const math::Vec2f p1Vec2f{p1.pos};
    const math::Vec2f p2Vec2f{p2.pos};
    const math::Vec2f p3Vec2f{p3.pos};

    const float ABC = edgeFunction(p1Vec2f, p2Vec2f, p3Vec2f);

    // Dont draw triangles that are back facing
    if (ABC <= 0)
        return;

    // Get the bounding box of the triangle
    const int minX = static_cast<int>(std::floor(std::min({p1.pos.x(), p2.pos.x(), p3.pos.x()})));
    const int maxX = static_cast<int>(std::ceil(std::max({p1.pos.x(), p2.pos.x(), p3.pos.x()})));
    const int minY = static_cast<int>(std::floor(std::min({p1.pos.y(), p2.pos.y(), p3.pos.y()})));
    const int maxY = static_cast<int>(std::ceil(std::max({p1.pos.y(), p2.pos.y(), p3.pos.y()})));

    // Loop through all the pixels of the bounding box
    for (int y = minY; y <= maxY; y++)
    {
        for (int x = minX; x <= maxX; x++)
        {

            if (isInsideTriangle({x, y}, p1Vec2f, p2Vec2f, p3Vec2f))
            {
                // Calculate our edge functions
                const float ABP = edgeFunction(p1Vec2f, p2Vec2f, {x, y});
                const float BCP = edgeFunction(p2Vec2f, p3Vec2f, {x, y});
                const float CAP = edgeFunction(p3Vec2f, p1Vec2f, {x, y});

                const float weightA = BCP / ABC;
                const float weightB = CAP / ABC;
                const float weightC = ABP / ABC;

                // Interpolate the colours at point P
                const std::uint8_t r = static_cast<uint8_t>(std::round(p1.color.r * weightA + p2.color.r * weightB + p3.color.r * weightC));
                const std::uint8_t g = static_cast<uint8_t>(std::round(p1.color.g * weightA + p2.color.g * weightB + p3.color.g * weightC));
                const std::uint8_t b = static_cast<uint8_t>(std::round(p1.color.b * weightA + p2.color.b * weightB + p3.color.b * weightC));
                const std::uint8_t a = static_cast<uint8_t>(std::round(p1.color.a * weightA + p2.color.a * weightB + p3.color.a * weightC));

                // Draw the pixel
                frameBuffer_.putPixel(x, y, {r, g, b, a});
            }
        }
    }
}
