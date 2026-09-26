#pragma once

#include "app/App.hpp"
#include "geometry/Cube.hpp"
#include "math/Helpers.h"
#include "math/Matrix.h"

#include <SDL3/SDL.h>
#include <chrono>
#include <numbers>

App::App(unsigned int width, unsigned int height)
    : display_(width, height), renderer_(width, height)
{
    keys_ = SDL_GetKeyboardState(nullptr);
    isRunning_ = display_.isValid();
}

void App::run()
{
    auto previous = std::chrono::steady_clock::now();

    while (isRunning_)
    {
        auto current = std::chrono::steady_clock::now();
        dt = current - previous;
        previous = current;

        renderer_.beginFrame();

        update();
        if (!isRunning_)
            break;

        composeFrame();

        display_.presentFrame(renderer_.getFramebuffer());
        if (!display_.isValid())
            isRunning_ = false;
    }
}

void App::update()
{
    // events: things that happen once
    while (SDL_PollEvent(&event_))
    {
        if (event_.type == SDL_EVENT_QUIT || event_.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
        {
            isRunning_ = false;
        }
    }

    // state: things that continue while held
    if (keys_[SDL_SCANCODE_W])
        thetaX_ = math::wrapAngle(thetaX_ - std::numbers::pi_v<float> * dt.count());
    if (keys_[SDL_SCANCODE_S])
        thetaX_ = math::wrapAngle(thetaX_ + std::numbers::pi_v<float> * dt.count());
    if (keys_[SDL_SCANCODE_A])
        thetaY_ = math::wrapAngle(thetaY_ - std::numbers::pi_v<float> * dt.count());
    if (keys_[SDL_SCANCODE_D])
        thetaY_ = math::wrapAngle(thetaY_ + std::numbers::pi_v<float> * dt.count());
    if (keys_[SDL_SCANCODE_Q])
        thetaZ_ = math::wrapAngle(thetaZ_ + std::numbers::pi_v<float> * dt.count());
    if (keys_[SDL_SCANCODE_E])
        thetaZ_ = math::wrapAngle(thetaZ_ - std::numbers::pi_v<float> * dt.count());
    if (keys_[SDL_SCANCODE_Z])
        zOffset_ -= dt.count();
    if (keys_[SDL_SCANCODE_X])
        zOffset_ += dt.count();
}

void App::composeFrame()
{
    static const geometry::Mesh baseCube = geometry::primitives::createMulticolorCube(0.5f);
    auto cube = baseCube;

    auto rotationX = math::Mat3f::Rotation<1, 2>(thetaX_);
    auto rotationY = math::Mat3f::Rotation<2, 0>(thetaY_);
    auto rotationZ = math::Mat3f::Rotation<0, 1>(thetaZ_);

    for (auto &vertex : cube.vertices)
    {
        vertex.pos = vertex.pos * rotationX * rotationY * rotationZ;
        vertex.pos += math::Vec3f{0.0f, 0.0f, 2.0f + zOffset_};
    }

    renderer_.drawTriangles(cube);
}
