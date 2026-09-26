#pragma once

#include "graphics/Framebuffer.hpp"
#include "math/Vector.h"
#include "geometry/Vertex.h"
namespace graphics
{

    class Rasterizer
    {
    public:
        explicit Rasterizer(Framebuffer &frameBuffer) : frameBuffer_(frameBuffer) {}

        // While vertex is technically 3D, we discard the Z component.
        // We use geometry::Vertex because it contains the color of each point which allows us to interpolate colors.
        void drawLine(const geometry::Vertex &p1, const geometry::Vertex &p2, const Pixel &color);
        void drawTriangle(const geometry::Vertex &p1, const geometry::Vertex &p2, const geometry::Vertex &p3);

    private:
        // Screen coordinates: x increases rightward, y downward. The directed edge
        // must belong to a triangle with positive signed area (clockwise on screen).
        [[nodiscard]] bool isTopLeftEdge(const math::Vec2f &start, const math::Vec2f &end);

        // Includes top/left boundaries, excludes bottom/right boundaries.
        // Accepts either winding; zero-area triangles contain no points.
        [[nodiscard]] bool isInsideTriangle(const math::Vec2f &point,
                                            const math::Vec2f &p1,
                                            const math::Vec2f &p2,
                                            const math::Vec2f &p3);

        [[nodiscard]] float edgeFunction(const math::Vec2f &start, const math::Vec2f &end, const math::Vec2f &point);

    private:
        Framebuffer &frameBuffer_;
    };

}
