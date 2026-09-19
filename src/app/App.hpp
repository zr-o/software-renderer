#pragma once

#include "display/Display.hpp"
#include "graphics/Renderer.hpp"

class App
{
public:
    App(unsigned int width, unsigned int height);

    void run();

private:
    void handleEvents();
    void composeFrame();

    // USER DEFINED FUNCTIONS

private:
    Display display_;
    graphics::Renderer renderer_;

    bool isRunning_;
    
    SDL_Event event_{};

    // USER DEFINED VARIABLES
};
