#pragma once

#include "display/Display.hpp"
#include "graphics/Renderer.hpp"

#include <chrono>

class App
{
public:
    App(unsigned int width, unsigned int height);

    void run();

private:
    void update();
    void composeFrame();

    // USER DEFINED FUNCTIONS

private:
    Display display_;
    graphics::Renderer renderer_;

    bool isRunning_;

    SDL_Event event_{}; // Events for 1 time usage (for example opening inventory)
    const bool* keys_ = nullptr;  // Contains an array of key states indexed by SDL scan codes

    // USER DEFINED VARIABLES
    float thetaX_{};
    float thetaY_{};
    float thetaZ_{};
    float zOffset_{};
    std::chrono::duration<float> dt{};
};
