#include "graphics/Framebuffer.hpp"

void Framebuffer::clear(const Pixel &color)
{
    for (Pixel &pixel : pixels_)
    {
        pixel = color;
    }
}

void Framebuffer::putPixel(unsigned int x, unsigned int y, const Pixel &color)
{
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        return;

    pixels_[y * width_ + x] = color;
}