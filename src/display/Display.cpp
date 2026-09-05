#pragma once

#include <iostream>
#include "display/Display.hpp"

Display::Display(int width, int height) : width_(width), height_(height)
{
    // Initialize SDL's video system
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return;
    }

    std::cout << "SDL initialized successfully!\n";

    if (!SDL_CreateWindowAndRenderer("CPU Rasterizer", width_, height_, SDL_WINDOW_RESIZABLE, &window_, &renderer_))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return;
    }
    std::cout << "SDL window and renderer initialized successfully!\n";

    texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width_, height_);
    if (!texture_)
    {
        std::cerr << "SDL_CreateTexture failed: " << SDL_GetError() << '\n';
    }
    std::cout << "SDL texture initialized successfully!\n";

    isRunning_ = true;
}

Display::~Display()
{
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Display::handleEvents()
{
    while (SDL_PollEvent(&event_))
    {
        if (event_.type == SDL_EVENT_QUIT)
        {
            isRunning_ = false;
        }
    }
}

FramebufferView Display::lockFrameBuffer()
{
    void *pixels;
    int pitch = 0;

    // Every frame, the array of pixels is at a different location which is why
    // we can't really use the address
    SDL_LockTexture(texture_, nullptr, &pixels, &pitch);

    return {pixels, width_, height_, pitch};
}

void Display::unlockFrameBuffer()
{
    SDL_UnlockTexture(texture_);
}

void Display::present()
{
    SDL_RenderTexture(renderer_, texture_, nullptr, nullptr);
    SDL_RenderPresent(renderer_);
}