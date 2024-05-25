#ifndef GAMESTATES_H
#define GAMESTATES_H

#include "bsp.h"
#include "player.h"
#include "segment_handler.h"
#include "structs.h"
#include "ui/def.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

extern bool firstTimeLaunching;

typedef void (*GameStateFunction)(engine *e);

extern GameStateFunction game_states_update[STATE_COUNT];
extern GameStateFunction game_states_init[STATE_COUNT];
extern GameStateFunction game_states_free[STATE_COUNT];

void switch_scene(engine *e, int scene);

void update_menu_state(engine *e);
void update_ingame_state(engine *e);

void init_menu_state(engine *e);
void init_ingame_state(engine *e);

void free_menu_state(engine *e);
void free_ingame_state(engine *e);

#endif
