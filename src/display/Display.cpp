#pragma once

#include <cstring>
#include <iostream>
#include "display/Display.hpp"

Display::Display(unsigned int width, unsigned int height)
    : width_(width), height_(height), frameBuffer_(width, height)
{
    // Initialize SDL's video system
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return;
    }

    std::cout << "SDL initialized successfully!\n";

    if (!SDL_CreateWindowAndRenderer("CPU Rasterizer", width_, height_, 0, &window_, &renderer_))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return;
    }
    std::cout << "SDL window and renderer initialized successfully!\n";

    texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width_, height_);
    if (!texture_)
    {
        std::cerr << "SDL_CreateTexture failed: " << SDL_GetError() << '\n';
        return;
    }
    std::cout << "SDL texture initialized successfully!\n";

    validState_ = true;
}

Display::~Display()
{
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Display::presentFrame() const
{
    if (!validState_)
        return;

    void *pixels = nullptr;
    int pitch = 0;

    // Every frame, the array of pixels is at a different location which is why
    // we can't really use the address
    if (!SDL_LockTexture(texture_, nullptr, &pixels, &pitch))
    {
        std::cerr << "SDL_LockTexture failed: " << SDL_GetError() << '\n';
        return;
    }

    const Pixel *sourcePixels = frameBuffer_.getRawPixelData();
    for (unsigned int y = 0; y < height_; ++y)
    {
        auto *destinationRow = static_cast<uint8_t *>(pixels) + y * pitch;
        const Pixel *sourceRow = sourcePixels + y * width_;
        std::memcpy(destinationRow, sourceRow, width_ * sizeof(Pixel));
    }

    SDL_UnlockTexture(texture_);

    SDL_RenderTexture(renderer_, texture_, nullptr, nullptr);
    SDL_RenderPresent(renderer_);
}

void Display::beginFrame()
{
    if (!validState_)
        return;
    frameBuffer_.clear(Colors::BlackPixel);
}

void Display::putPixel(unsigned int x, unsigned int y, const Pixel &color)
{
    frameBuffer_.putPixel(x, y, color);
}