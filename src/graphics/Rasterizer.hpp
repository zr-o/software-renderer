#pragma once

#include "graphics/Framebuffer.hpp"

namespace graphics
{

class Rasterizer
{
public:
    explicit Rasterizer(Framebuffer &frameBuffer) : frameBuffer_(frameBuffer) {}

    void drawLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, const Pixel &color);

private:
    Framebuffer &frameBuffer_;
};

}
