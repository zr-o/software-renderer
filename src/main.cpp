#include <SDL3/SDL.h>

#include <iostream>
#include "Pixel.hpp"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

int main()
{
    // Initialize SDL's video system
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return 1;
    }

    std::cout << "SDL initialized successfully!\n";

    // Create a window, a texture and the renderer context
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    if (!SDL_CreateWindowAndRenderer("CPU Rasterizer", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 1;
    }
    std::cout << "SDL window and renderer initialized successfully!\n";

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!texture)
    {
        std::cerr << "SDL_CreateTexture failed: " << SDL_GetError() << '\n';
        return 1;
    }
    std::cout << "SDL texture initialized successfully!\n";

    // Keep the program running until the window is closed
    bool rendererIsRunning = true;
    SDL_Event event;

    // SDL will give us access to the raw pixel data of the texture, which we can then manipulate directly.
    void *pixels;
    int pitch;

    // Square properties
    int squareX = 0;
    constexpr int squareY = 200;
    constexpr int squareSize = 100;

    while (rendererIsRunning)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                rendererIsRunning = false;
            }
        }

        if (SDL_LockTexture(texture, nullptr, &pixels, &pitch))
        {
            // 1. Clear the whole screen
            for (int y = 0; y < WINDOW_HEIGHT; ++y)
            {
                Pixel *row = reinterpret_cast<Pixel *>(
                    static_cast<uint8_t *>(pixels) + y * pitch);

                for (int x = 0; x < WINDOW_WIDTH; ++x)
                {
                    row[x] = {0, 0, 0, 255};
                }
            }

            // 2. Draw square
            for (int y = squareY; y < squareY + squareSize; ++y)
            {
                Pixel *row = reinterpret_cast<Pixel *>(
                    static_cast<uint8_t *>(pixels) + y * pitch);

                for (int x = squareX; x < squareX + squareSize; ++x)
                {
                    row[x] = {255, 0, 0, 255};
                }
            }

            SDL_UnlockTexture(texture);
        }

        // 3. Move square
        squareX += 1;

        // 4. Put it back on the left when it leaves the screen
        if (squareX + squareSize >= WINDOW_WIDTH)
        {
            squareX = 0;
        }

        // 5. Display
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        // 6. Delay to limit frame rate
        SDL_Delay(1);
    }

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}