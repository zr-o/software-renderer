#pragma once

#include "graphics/Framebuffer.hpp"
#include <SDL3/SDL.h>

class Display
{
public:
    Display(unsigned int width, unsigned int height);
    ~Display();

    Display(const Display &) = delete;
    Display &operator=(const Display &) = delete;

    void beginFrame();
    void presentFrame() const;

    void putPixel(unsigned x, unsigned y, const Pixel &color);
    [[nodiscard]] bool isValid() const { return validState_; }

private:
    const unsigned int width_;
    const unsigned int height_;

    Framebuffer frameBuffer_;

    // Create a window, a texture, event and the renderer context
    SDL_Renderer *renderer_ = nullptr;
    SDL_Texture *texture_ = nullptr;
    SDL_Window *window_ = nullptr;

    bool validState_ = false;
};