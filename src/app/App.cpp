#include "app/App.hpp"

#include <array>
#include <cmath>
#include <cstdint>

#include <SDL3/SDL.h>

#include "math/Matrix.h"
#include "geometry/Cube.hpp"
#include "graphics/ScreenTransformer.hpp"

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
    geometry::Mesh cube = geometry::primitives::createCube(0.5f);

    for (auto &vertex : cube.vertices)
    {
        graphics::ScreenTransformer::transformVector(vertex, width_, height_);
    }

    for (std::size_t i = 0; i + 1 < cube.indices.size(); i += 2)
    {
        const math::Vec3f &firstVertex = cube.vertices[cube.indices[i]];
        const math::Vec3f &secondVertex = cube.vertices[cube.indices[i + 1]];

        display_.putLine(static_cast<unsigned>(firstVertex.elements[0]),
                         static_cast<unsigned>(firstVertex.elements[1]),
                         static_cast<unsigned>(secondVertex.elements[0]),
                         static_cast<unsigned>(secondVertex.elements[1]),
                         graphics::Colors::WhitePixel);
    }
}

