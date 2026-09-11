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
    void drawLine(int x0, int y0, int x1, int y1, const Pixel& color);

    Display display_;
    unsigned int width_;
    unsigned int height_;
    bool isRunning_ = true;
    bool rotationPaused_ = false;
    float rotationAngle_ = 0.0f;
    uint64_t lastFrameTime_ = 0;
};