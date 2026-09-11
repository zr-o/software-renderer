#include "app/App.hpp"

#include <array>
#include <cmath>
#include <cstdint>

#include <SDL3/SDL.h>

#include "math/Mat2.h"


App::App(unsigned int width, unsigned int height)
    : display_(width, height), width_(width), height_(height)
{
    isRunning_ = display_.isValid();
}

void App::run()
{
    while (isRunning_)
    {
        display_.beginFrame();

        handleEvents();
        if (!isRunning_)
            break;

        composeFrame();

        display_.presentFrame();
        if (!display_.isValid())
            isRunning_ = false;
            
        SDL_Delay(1);
    }
}

void App::handleEvents()
{
    while (SDL_PollEvent(&event_))
    {
        if (event_.type == SDL_EVENT_QUIT || event_.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
        {
            isRunning_ = false;
        }
    }
}

void App::composeFrame()
{
}
