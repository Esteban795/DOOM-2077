#ifndef GAMESTATES_H
#define GAMESTATES_H
#define STATE_COUNT 4

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../include/engine.h"

typedef enum {
    STATE_MENU,
    STATE_INGAME,
    STATE_PAUSE,
    STATE_GAMEOVER
} GameState;

typedef struct {
    SDL_Renderer *renderer;
    bool *isFirstTime;
    bool isRunning;
    int numkeys;
    const uint8_t *keys;
    engine *e;
    
} GameStateArgs;

typedef void (*GameStateFunction)(GameStateArgs *args);

extern GameStateFunction game_states[STATE_COUNT];




// void handle_menu_state(SDL_Renderer *renderer);
// void handle_ingame_state(SDL_Renderer *renderer);
// void handle_paused_state(SDL_Renderer *renderer);
// void handle_gameover_state(SDL_Renderer *renderer);

#endif
