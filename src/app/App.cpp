#include "app/App.hpp"

#include <SDL3/SDL.h>

App::App(unsigned int width, unsigned int height)
    : display_(width, height), width_(width), height_(height)
{
    isRunning_ = display_.isValid();
}

void App::run()
{
    while (isRunning_)
    {
        handleEvents();
        if (!isRunning_)
            break;

        display_.beginFrame();
        draw();
        display_.presentFrame();
        SDL_Delay(16);
    }
}

void App::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
            isRunning_ = false;
    }
}

void App::draw()
{
    constexpr unsigned int squareSize = 50;
    const unsigned int squareX = (width_ - squareSize) / 2;
    const unsigned int squareY = (height_ - squareSize) / 2;

    for (unsigned int y = squareY; y < squareY + squareSize; ++y)
    {
        for (unsigned int x = squareX; x < squareX + squareSize; ++x)
            display_.putPixel(x, y, Colors::RedPixel);
    }
}