#pragma once

#include "graphics/FramebufferView.hpp"
#include "graphics/Pixel.hpp"

class Rasterizer
{
public:
    Rasterizer() = default;
    Rasterizer(const Rasterizer &) = delete;
    Rasterizer &operator=(const Rasterizer &) = delete;

    // This must be done before manipulating each frame because of how
    // SDL works (temporary array access of the pixels and changes everytime I lock)
    void setFrameBuffer(FramebufferView frameBuffer) { frameBuffer_ = frameBuffer; }

    void drawPoint(int x, int y, Pixel &pixel);

private:
    FramebufferView frameBuffer_;
};