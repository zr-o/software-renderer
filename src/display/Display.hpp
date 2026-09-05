#pragma once

#include "graphics/FramebufferView.hpp"
#include <SDL3/SDL.h>

class Display
{
public:
    Display(int width, int height);
    ~Display();

    FramebufferView lockFrameBuffer();
    void unlockFrameBuffer();
    void present();
    void handleEvents();
    bool isRunning() { return isRunning_; }

private:
    int width_;
    int height_;

    // Create a window, a texture, event and the renderer context
    SDL_Renderer *renderer_;
    SDL_Texture *texture_;
    SDL_Window *window_;
    SDL_Event event_;

    bool isRunning_ = false;
};