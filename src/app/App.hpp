#pragma once

#include "display/Display.hpp"

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
    unsigned int width_;
    unsigned int height_;
    bool isRunning_;
    SDL_Event event_{};

    // USER DEFINED VARIABLES
};