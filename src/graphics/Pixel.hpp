#pragma once

#include <cstdint>

namespace graphics
{

struct Pixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    constexpr Pixel() 
        : r(0), g(0), b(0), a(255) {}

    constexpr Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255u)
        : r(red), g(green), b(blue), a(alpha) {}

    constexpr Pixel(const Pixel &otherPixel) 
        : r(otherPixel.r), g(otherPixel.g), b(otherPixel.b), a(otherPixel.a) {}

    constexpr Pixel(const Pixel &otherPixel, uint8_t alpha) 
        : r(otherPixel.r), g(otherPixel.g), b(otherPixel.b), a(alpha) {}
};

namespace Colors
{
    inline constexpr Pixel RedPixel {255u,   0u,   0u};
    inline constexpr Pixel GreenPixel {  0u, 255u,   0u};
    inline constexpr Pixel BluePixel  {  0u,   0u, 255u};
    inline constexpr Pixel BlackPixel {  0u,   0u,   0u};
    inline constexpr Pixel WhitePixel {255u, 255u, 255u};
}

}