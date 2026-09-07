#include "graphics/Rasterizer.hpp"

// We check the bounds incase the input is not valid
void Rasterizer::drawPoint(int x, int y, Pixel &pixel)
{
    if (x < 0 || x >= frameBuffer_.width || y < 0 || y >= frameBuffer_.height)
    {
        return;
    }

    frameBuffer_[y][x] = pixel;
}