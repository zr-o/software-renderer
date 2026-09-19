#include "graphics/Rasterizer.hpp"

#include <algorithm>
#include <cmath>

void graphics::Rasterizer::drawLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, const Pixel &color)
{
    const int dx = static_cast<int>(x1) - static_cast<int>(x0);
    const int dy = static_cast<int>(y1) - static_cast<int>(y0);
    const int step = std::max(std::abs(dx), std::abs(dy));

    if (step != 0)
    {
        const float stepX = static_cast<float>(dx) / step;
        const float stepY = static_cast<float>(dy) / step;

        for (int i = 0; i < step + 1; i++)
        {
            frameBuffer_.putPixel(static_cast<int>(std::round(x0 + i * stepX)),
                                  static_cast<int>(std::round(y0 + i * stepY)), color);
        }
    }
}
