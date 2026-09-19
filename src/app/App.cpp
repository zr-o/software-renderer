#include "app/App.hpp"

#include <SDL3/SDL.h>

#include "geometry/Cube.hpp"

App::App(unsigned int width, unsigned int height)
    : display_(width, height), renderer_(width, height)
{
    isRunning_ = display_.isValid();
}

void App::run()
{
    while (isRunning_)
    {
        renderer_.beginFrame();

        handleEvents();
        if (!isRunning_)
            break;

        composeFrame();

        display_.presentFrame(renderer_.getFramebuffer());
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
    const geometry::Mesh cube = geometry::primitives::createCube(0.5f);
    renderer_.drawWireframe(cube);
}

