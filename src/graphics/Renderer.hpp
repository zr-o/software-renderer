#pragma once

#include "geometry/Mesh.hpp"
#include "graphics/Framebuffer.hpp"
#include "graphics/Rasterizer.hpp"

namespace graphics
{

class Renderer
{
public:
    Renderer(unsigned int width, unsigned int height);

    void beginFrame(const Pixel &clearColor = Colors::BlackPixel);
    // Each pair of mesh indices describes a line segment.
    void drawWireframe(const geometry::Mesh &mesh, const Pixel &color = Colors::WhitePixel);

    [[nodiscard]] const Framebuffer &getFramebuffer() const { return frameBuffer_; }

private:
    Framebuffer frameBuffer_;
    Rasterizer rasterizer_;
};

}
