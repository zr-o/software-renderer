#include <SDL3/SDL.h>

#include <iostream>

int main()
{
    // Initialize SDL's video system
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "Software Renderer",
        800,
        600,
        0
    );

    if (window == nullptr)
    {
        std::cerr << "SDL_CreateWindow failed: "
                  << SDL_GetError() << '\n';

        SDL_Quit();
        return 1;
    }

    std::cout << "SDL initialized successfully!\n";

    // Keep the program running until the window is closed
    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        SDL_Delay(16);
    }

    // Cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}