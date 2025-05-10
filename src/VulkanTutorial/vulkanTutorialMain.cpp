//
// Created by alejandro on 06/05/2025.
//

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <VulkanTutorial/vulkanTutorial.h>


struct AppState {
    SDL_Window *window;
    SDL_Renderer *renderer;
    VkSurfaceKHR* surface;
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    SDL_Init(SDL_INIT_VIDEO);
    auto *state = new AppState;
    Uint32 count = 0;
    char const* const *extensions = SDL_Vulkan_GetInstanceExtensions(&count);
    vkt vt;
    vt.init(extensions, count);

    SDL_CreateWindowAndRenderer("Vulkan", 800, 600, SDL_WINDOW_VULKAN, &state->window, &state->renderer);
    SDL_Vulkan_CreateSurface(state->window, static_cast<VkInstance>(*vt.getInstance()), nullptr, state->surface);
    *appstate = state;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    auto state = static_cast<AppState *>(appstate);
    SDL_SetRenderDrawColor(state->renderer, 0xFF, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderPresent(state->renderer);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event *event) {

    return event->type == SDL_EVENT_QUIT ? SDL_APP_FAILURE : SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    auto state = static_cast<AppState *>(appstate);
    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
    free(state);
    SDL_Quit();
}