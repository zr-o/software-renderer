#pragma once

#include <cstdint>
#include "Pixel.hpp"

struct FramebufferView
{
    void *pixels;
    int width;
    int height;
    int pitch;

    Pixel *operator[](int y)
    {
        return reinterpret_cast<Pixel *>(static_cast<uint8_t *>(pixels) + y * pitch);
    }
};