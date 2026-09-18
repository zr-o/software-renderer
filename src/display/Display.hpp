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
    void presentFrame();

    [[nodiscard]] constexpr unsigned int getWidth() const { return width_; }
    [[nodiscard]] constexpr unsigned int getHeight() const { return height_; }

    void putPixel(unsigned int x, unsigned int y, const graphics::Pixel &color);
    void putLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, const graphics::Pixel &color);
    [[nodiscard]] bool isValid() const { return validState_; }

private:
    const unsigned int width_;
    const unsigned int height_;

    graphics::Framebuffer frameBuffer_;

    // Create a window, a texture, event and the renderer context
    SDL_Renderer *renderer_ = nullptr;
    SDL_Texture *texture_ = nullptr;
    SDL_Window *window_ = nullptr;

    bool validState_ = false;
};