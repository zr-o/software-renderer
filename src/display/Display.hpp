#pragma once

#include "graphics/FramebufferView.hpp"
#include <SDL3/SDL.h>

class Display
{
public:
    Display(int width, int height);
    ~Display();

    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;

    FramebufferView lockFrameBuffer() const;
    void unlockFrameBuffer() const;
    void presentFrameBuffer() const;
    void handleEvents();
    bool isRunning() { return isRunning_; }

private:
    int width_;
    int height_;

    // Create a window, a texture, event and the renderer context
    SDL_Renderer *renderer_ = nullptr;
    SDL_Texture *texture_ = nullptr;
    SDL_Window *window_ = nullptr;
    SDL_Event event_;

    bool isRunning_ = false;
};