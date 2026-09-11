#include "app/App.hpp"

#include <array>
#include <cmath>
#include <cstdint>

#include <SDL3/SDL.h>

#include "math/Mat2.h"

namespace
{
    constexpr float Pi = 3.14159265358979323846f;
    constexpr Pixel StarColor{255u, 220u, 40u};
}

App::App(unsigned int width, unsigned int height)
    : display_(width, height), width_(width), height_(height)
{
    isRunning_ = display_.isValid();
}

void App::run()
{
    lastFrameTime_ = SDL_GetTicks();

    while (isRunning_)
    {
        display_.beginFrame();
        handleEvents();
        if (!isRunning_)
            break;

        const uint64_t currentTime = SDL_GetTicks();
        const float deltaTime = static_cast<float>(currentTime - lastFrameTime_) / 1000.0f;
        lastFrameTime_ = currentTime;

        if (!rotationPaused_)
            rotationAngle_ += deltaTime;

        composeFrame();
        display_.presentFrame();
        SDL_Delay(1);
    }
}

void App::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            isRunning_ = false;
        }
        else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_SPACE)
        {
            rotationPaused_ = true;
        }
        else if (event.type == SDL_EVENT_KEY_UP && event.key.key == SDLK_SPACE)
        {
            rotationPaused_ = false;
        }
    }
}

void App::composeFrame()
{
    constexpr float outerRadius = 180.0f;
    constexpr float innerRadius = 75.0f;
    constexpr float centerAngle = -Pi / 2.0f;

    const Matrix2D rotation = Matrix2D::Rotation(rotationAngle_);
    const Vector2D center{
        static_cast<float>(width_) / 2.0f,
        static_cast<float>(height_) / 2.0f};

    std::array<Vector2D, 10> starPoints{};
    for (std::size_t index = 0; index < starPoints.size(); ++index)
    {
        const float radius = index % 2 == 0 ? outerRadius : innerRadius;
        const float angle = centerAngle + static_cast<float>(index) * Pi / 5.0f;
        starPoints[index] = center + Vector2D{std::cos(angle) * radius,
                                              std::sin(angle) * radius} * rotation;
    }

    for (std::size_t index = 0; index < starPoints.size(); ++index)
    {
        const Vector2D& start = starPoints[index];
        const Vector2D& end = starPoints[(index + 1) % starPoints.size()];
        drawLine(static_cast<int>(start.x), static_cast<int>(start.y),
                 static_cast<int>(end.x), static_cast<int>(end.y), StarColor);
    }
}

void App::drawLine(int x0, int y0, int x1, int y1, const Pixel& color)
{
    const int deltaX = std::abs(x1 - x0);
    const int stepX = x0 < x1 ? 1 : -1;
    const int deltaY = -std::abs(y1 - y0);
    const int stepY = y0 < y1 ? 1 : -1;
    int error = deltaX + deltaY;

    while (true)
    {
        display_.putPixel(static_cast<unsigned int>(x0), static_cast<unsigned int>(y0), color);

        if (x0 == x1 && y0 == y1)
            break;

        const int doubledError = 2 * error;
        if (doubledError >= deltaY)
        {
            error += deltaY;
            x0 += stepX;
        }
        if (doubledError <= deltaX)
        {
            error += deltaX;
            y0 += stepY;
        }
    }
}