#ifndef ENGINE_H
#define ENGINE_H

#include "bsp.h"
#include "map_renderer.h"
#include "player.h"
#include "structs.h"
#include "segment_handler.h"
#include "events.h"
#include "game_states.h"
#include "timer.h"
engine *init_engine(const char *wadPath, SDL_Renderer *renderer);

int update_engine(engine *e,int dt);

void read_map(engine* e, SDL_Renderer *renderer,char* map_name);

void engine_free(engine *e);
#endif