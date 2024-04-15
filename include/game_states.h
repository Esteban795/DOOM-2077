#ifndef GAMESTATES_H
#define GAMESTATES_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "structs.h"
#include "player.h"
#include "bsp.h"
#include "segment_handler.h"

extern bool firstTimeLaunching;

typedef void (*GameStateFunction)(engine* e);

extern GameStateFunction game_states[STATE_COUNT];

void handle_menu_state(engine* e);
void handle_settings_state(engine* e);
void handle_ingame_state(engine* e);
void handle_paused_state(engine* e);
void handle_gameover_state(engine* e);

#endif