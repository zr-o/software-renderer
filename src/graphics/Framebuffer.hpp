#pragma once

#include <cstdint>
#include "Pixel.hpp"
#include <vector>

class Framebuffer
{
public:
    Framebuffer(unsigned int width, unsigned int height)
        : width_(width), height_(height), pixels_(width * height) {}

    Framebuffer(const Framebuffer &) = delete;
    Framebuffer &operator=(const Framebuffer &) = delete;

    void clear(const Pixel& color = Colors::BlackPixel);
    void putPixel(unsigned int x, unsigned int y, const Pixel& color);

    [[nodiscard]] unsigned int getWidth() const { return width_; }
    [[nodiscard]] unsigned int getHeight() const { return height_; }
    [[nodiscard]] Pixel *getRawPixelData() { return pixels_.data(); }
    [[nodiscard]] const Pixel *getRawPixelData() const { return pixels_.data(); }

private:
    const unsigned int width_;
    const unsigned int height_;
    std::vector<Pixel> pixels_;
};