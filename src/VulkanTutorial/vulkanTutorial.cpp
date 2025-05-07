#include "VulkanTutorial/vulkanTutorial.h"

#include <iostream>
#include <vulkan/vulkan.hpp>
#include <SDL3/SDL.h>

void hello() {
    std::cout << "Hello World!" << std::endl;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_CreateWindowAndRenderer("Hellooooo", 800, 600, 0, &window, &renderer);
    std::cout << "Hello World!" << std::endl;
    if (window == NULL) {
        std::cout << "Failed to create window" << std::endl;
        return;
    }
    bool quit = false;
    while (!quit) {
        SDL_Event e;
        SDL_WaitEvent(&e);
        if (e.type == SDL_EVENT_QUIT) {
            quit = true;
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Hello, World!" << std::endl;
}