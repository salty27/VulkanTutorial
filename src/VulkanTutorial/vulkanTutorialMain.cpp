//
// Created by alejandro on 06/05/2025.
//

#define SDL_MAIN_USE_CALLBACKS
#include <stdlib.h>
#include <SDL3/SDL_main.h>

#include <SDL3/SDL.h>


struct AppState {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    SDL_Init(SDL_INIT_VIDEO);
    auto *state = new AppState;
    SDL_CreateWindowAndRenderer("HONK", 800, 600, 0, &state->window, &state->renderer);
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