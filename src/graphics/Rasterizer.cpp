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

// Deprecated function, drawTriangles doesnt use it anymore.
std::pair<bool, std::tuple<float, float, float>> graphics::Rasterizer::isInsideTriangle(const math::Vec2f &point,
                                                                                        const math::Vec2f &p1,
                                                                                        const math::Vec2f &p2,
                                                                                        const math::Vec2f &p3)
{
    constexpr float tolerancePixels = 1e-5f;

    auto passesEdge = [&](const math::Vec2f &a, const math::Vec2f &b) -> std::pair<bool, float>
    {
        const float edge = edgeFunction(a, b, point);

        const float dx = b.x() - a.x();
        const float dy = b.y() - a.y();

        const float edgeLength = std::sqrt(dx * dx + dy * dy);
        const float epsilon = tolerancePixels * edgeLength;

        return {edge > epsilon ||
                    (std::abs(edge) <= epsilon && isTopLeftEdge(a, b)),
                edge};
    };

    auto [con1, e1] = passesEdge(p1, p2);
    if (!con1)
        return {false, {0.f, 0.f, 0.f}};

    auto [con2, e2] = passesEdge(p2, p3);
    if (!con2)
        return {false, {0.f, 0.f, 0.f}};

    auto [con3, e3] = passesEdge(p3, p1);
    return {con3, {e1, e2, e3}};
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

void graphics::Rasterizer::drawTriangle(const geometry::Vertex &p1, const geometry::Vertex &p2, const geometry::Vertex &p3, bool usingBackfaceCulling)
{
    const math::Vec2f p1Vec2f{p1.pos};
    const math::Vec2f p2Vec2f{p2.pos};
    const math::Vec2f p3Vec2f{p3.pos};

    const float ABC = edgeFunction(p1Vec2f, p2Vec2f, p3Vec2f);

    // Dont draw triangles that are back facing only if backfaceCulling is activated
    if (ABC < 0.f)
    {
        if (usingBackfaceCulling)
            return;

        // Reverse the winding so the edge tests work.
        drawTriangle(p1, p3, p2, false);
        return;
    }

    // Get the bounding box of the triangle
    const int minX = static_cast<int>(std::floor(std::min({p1.pos.x(), p2.pos.x(), p3.pos.x()})));
    const int maxX = static_cast<int>(std::ceil(std::max({p1.pos.x(), p2.pos.x(), p3.pos.x()})));
    const int minY = static_cast<int>(std::floor(std::min({p1.pos.y(), p2.pos.y(), p3.pos.y()})));
    const int maxY = static_cast<int>(std::ceil(std::max({p1.pos.y(), p2.pos.y(), p3.pos.y()})));

    auto calculateConstants = [&](const math::Vec2f &a, const math::Vec2f &b) -> std::tuple<float, float, float>
    {
        constexpr float tolerancePixels = 1e-5f;

        const float dx = b.x() - a.x();
        const float dy = b.y() - a.y();

        const float edgeLength = std::sqrt(dx * dx + dy * dy);
        const float epsilon = tolerancePixels * edgeLength;

        return {dx, dy, epsilon};
    };

    // Represents the 3 edge function values of each edge that we will modify by dx and dy for each pixel
    // Without having to recalculate. Go see pineda algo for more info
    float ABP = edgeFunction(p1Vec2f, p2Vec2f, {static_cast<float>(minX) + 0.5f, static_cast<float>(minY) + 0.5f});
    float BCP = edgeFunction(p2Vec2f, p3Vec2f, {static_cast<float>(minX) + 0.5f, static_cast<float>(minY) + 0.5f});
    float CAP = edgeFunction(p3Vec2f, p1Vec2f, {static_cast<float>(minX) + 0.5f, static_cast<float>(minY) + 0.5f});



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
            if ((ABPconstY > epsilonAB || (std::abs(ABPconstY) <= epsilonAB && isTopLeftEdge(p1Vec2f, p2Vec2f))) && (BCPconstY > epsilonBC || (std::abs(BCPconstY) <= epsilonBC && isTopLeftEdge(p2Vec2f, p3Vec2f))) && (CAPconstY > epsilonCA || (std::abs(CAPconstY) <= epsilonCA && isTopLeftEdge(p3Vec2f, p1Vec2f))))
            {
                // Calculate our weight
                const float weightA = BCPconstY / ABC;
                const float weightB = CAPconstY / ABC;
                const float weightC = ABPconstY / ABC;

                // Interpolate the colours at point P
                const std::uint8_t r = static_cast<uint8_t>(std::round(p1.color.r * weightA + p2.color.r * weightB + p3.color.r * weightC));
                const std::uint8_t g = static_cast<uint8_t>(std::round(p1.color.g * weightA + p2.color.g * weightB + p3.color.g * weightC));
                const std::uint8_t b = static_cast<uint8_t>(std::round(p1.color.b * weightA + p2.color.b * weightB + p3.color.b * weightC));
                const std::uint8_t a = static_cast<uint8_t>(std::round(p1.color.a * weightA + p2.color.a * weightB + p3.color.a * weightC));

                // Draw the pixel
                frameBuffer_.putPixel(x, y, {r, g, b, a});
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
