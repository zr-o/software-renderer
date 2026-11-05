#include "graphics/Rasterizer.hpp"
#include "graphics/Fragment.hpp"
#include "math/Geometry.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>

// This assumes that the triangle follows CCW vertex orientation
// How ever this is calculated like its CW vertex rotation because on the canvas
// its inversed
bool graphics::Rasterizer::isTopLeftEdge(const math::Vec2f &start,
                                         const math::Vec2f &end)
{
    return end.y() < start.y() || (end.y() == start.y() && end.x() > start.x());
}

void graphics::Rasterizer::drawLine(const geometry::Vertex &p1,
                                    const geometry::Vertex &p2,
                                    const Pixel &color)
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
        frameBuffer_.putPixel(static_cast<unsigned int>(x),
                              static_cast<unsigned int>(y), color);
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

void graphics::Rasterizer::drawTriangle(
    const geometry::Vertex &p1, const geometry::Vertex &p2,
    const geometry::Vertex &p3,
    const std::function<void(const Fragment &)> &processFragment)
{
    const math::Vec2f p1Vec2f{p1.pos};
    const math::Vec2f p2Vec2f{p2.pos};
    const math::Vec2f p3Vec2f{p3.pos};

    const float ABC = math::edgeFunction(p1Vec2f, p2Vec2f, p3Vec2f);
    if (std::abs(ABC) < 1e-6f)
        return;

    // Get the bounding box of the triangle, clamped to the viewport
    const int minX = std::max(0, static_cast<int>(std::floor(std::min(
                                     {p1.pos.x(), p2.pos.x(), p3.pos.x()}))));

    const int maxX = std::min(static_cast<int>(width_) - 1,
                              static_cast<int>(std::ceil(std::max(
                                  {p1.pos.x(), p2.pos.x(), p3.pos.x()}))));

    const int minY = std::max(0, static_cast<int>(std::floor(std::min(
                                     {p1.pos.y(), p2.pos.y(), p3.pos.y()}))));

    const int maxY = std::min(static_cast<int>(height_) - 1,
                              static_cast<int>(std::ceil(std::max(
                                  {p1.pos.y(), p2.pos.y(), p3.pos.y()}))));
    if (minX > maxX || minY > maxY)
        return;

    auto calculateConstants =
        [&](const math::Vec2f &a,
            const math::Vec2f &b) -> std::tuple<float, float, float>
    {
        constexpr float tolerancePixels = 1e-5f;

        const float dx = b.x() - a.x();
        const float dy = b.y() - a.y();

        const float edgeLength = std::sqrt(dx * dx + dy * dy);
        const float epsilon = tolerancePixels * edgeLength;

        return {dx, dy, epsilon};
    };

    // Represents the 3 edge function values of each edge that we will modify by
    // dx and dy for each pixel Without having to recalculate. Go see pineda
    // algo for more info
    float ABP = math::edgeFunction(
        p1Vec2f, p2Vec2f,
        {static_cast<float>(minX) + 0.5f, static_cast<float>(minY) + 0.5f});
    float BCP = math::edgeFunction(
        p2Vec2f, p3Vec2f,
        {static_cast<float>(minX) + 0.5f, static_cast<float>(minY) + 0.5f});
    float CAP = math::edgeFunction(
        p3Vec2f, p1Vec2f,
        {static_cast<float>(minX) + 0.5f, static_cast<float>(minY) + 0.5f});

    auto const [dxAB, dyAB, epsilonAB] = calculateConstants(p1Vec2f, p2Vec2f);
    auto const [dxBC, dyBC, epsilonBC] = calculateConstants(p2Vec2f, p3Vec2f);
    auto const [dxCA, dyCA, epsilonCA] = calculateConstants(p3Vec2f, p1Vec2f);

    // Loop through all the pixels of the bounding box
    for (int y = minY; y <= maxY; y++)
    {
        // We do that so the edge values reset to x = minX at each row.
        float ABPconstY = ABP;
        float BCPconstY = BCP;
        float CAPconstY = CAP;

        for (int x = minX; x <= maxX; x++)
        {
            if ((ABPconstY > epsilonAB || (std::abs(ABPconstY) <= epsilonAB &&
                                           isTopLeftEdge(p1Vec2f, p2Vec2f))) &&
                (BCPconstY > epsilonBC || (std::abs(BCPconstY) <= epsilonBC &&
                                           isTopLeftEdge(p2Vec2f, p3Vec2f))) &&
                (CAPconstY > epsilonCA || (std::abs(CAPconstY) <= epsilonCA &&
                                           isTopLeftEdge(p3Vec2f, p1Vec2f))))
            {
                // Calculate our weight
                const float weightA = BCPconstY / ABC;
                const float weightB = CAPconstY / ABC;
                const float weightC = ABPconstY / ABC;

                // Interpolate the colours at point P
                graphics::Color interpolatedColor = (p1.color * weightA) +
                                                    (p2.color * weightB) +
                                                    (p3.color * weightC);
                graphics::Fragment newFragment = {
                    .x = static_cast<unsigned int>(x),
                    .y = static_cast<unsigned int>(y),
                    .depth = 0.0f,
                    .color = interpolatedColor};

                processFragment(newFragment);
                // Draw the pixel
                // frameBuffer_.putPixel(x, y, {r, g, b, a});
            }

            // Move by -dy the e values
            ABPconstY -= dyAB;
            BCPconstY -= dyBC;
            CAPconstY -= dyCA;
        }
        // Move by dx the e values
        ABP += dxAB;
        BCP += dxBC;
        CAP += dxCA;
    }
}
