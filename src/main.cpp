#include <SDL3/SDL.h>

#include <iostream>
#include "graphics/Rasterizer.hpp"
#include "display/Display.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

int main()
{
    Display display(WINDOW_WIDTH, WINDOW_HEIGHT);
    Rasterizer rasterizer;

    constexpr int squareSize = 50;
    Pixel squareColor(255, 0, 0);

    while (display.isRunning())
    {
        display.handleEvents();

        FramebufferView frameBuffer = display.lockFrameBuffer();
        if (frameBuffer.pixels == nullptr)
        {
            break;
        }

        rasterizer.setFrameBuffer(frameBuffer);

        Pixel backgroundColor(0, 0, 0);
        for (int y = 0; y < frameBuffer.height; ++y)
        {
            for (int x = 0; x < frameBuffer.width; ++x)
            {
                rasterizer.drawPoint(x, y, backgroundColor);
            }
        }

        const int squareX = (frameBuffer.width - squareSize) / 2;
        const int squareY = (frameBuffer.height - squareSize) / 2;
        for (int y = squareY; y < squareY + squareSize; ++y)
        {
            for (int x = squareX; x < squareX + squareSize; ++x)
            {
                rasterizer.drawPoint(x, y, squareColor);
            }
        }

        display.unlockFrameBuffer();
        display.presentFrameBuffer();
        SDL_Delay(16);
    }
    return 0;
}