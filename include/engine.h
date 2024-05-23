#ifndef ENGINE_H
#define ENGINE_H

#include "bsp.h"
#include "events.h"
#include "game_states.h"
#include "map_renderer.h"
#include "player.h"
#include "structs.h"
#include "segment_handler.h"
#include "structs.h"
#include "timer.h"

#include "ui/def.h"
#include "ui/event_handler.h"
engine *init_engine(const char *wadPath, SDL_Renderer *renderer);

int update_engine(engine *e, int dt);

// reads `map_name` from the wad file and initializes the wadData properly
void read_map(engine *e, char *map_name);

void engine_free(engine *e);
#endif
