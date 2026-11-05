#pragma once

#include "graphics/Fragment.hpp"
#include "graphics/Framebuffer.hpp"

#include "math/Vector.hpp"

#include "geometry/Vertex.hpp"

#include <functional>

namespace graphics
{

class Rasterizer
{
  public:
    explicit Rasterizer(Framebuffer &frameBuffer, unsigned int width,
                        unsigned int height)
        : frameBuffer_(frameBuffer), width_(width), height_(height)
    {
    }

    // While vertex is technically 3D, we discard the Z component.
    // We use geometry::Vertex because it contains the color of each point which
    // allows us to interpolate colors.
    void drawLine(const geometry::Vertex &p1, const geometry::Vertex &p2,
                  const Pixel &color);
    void
    drawTriangle(const geometry::Vertex &p1, const geometry::Vertex &p2,
                 const geometry::Vertex &p3,
                 const std::function<void(const Fragment &)> &processFragment);

  private:
    // Screen coordinates: x increases rightward, y downward. The directed edge
    // must belong to a triangle with positive signed area (clockwise on
    // screen).
    [[nodiscard]] bool isTopLeftEdge(const math::Vec2f &start,
                                     const math::Vec2f &end);

    Framebuffer &frameBuffer_; // Will soon be removed.
    unsigned int width_;
    unsigned int height_;
};

} // namespace graphics
