#ifndef ENGINE_H
#define ENGINE_H

#include "bsp.h"
#include "map_renderer.h"
#include "player.h"
#include "structs.h"
#include "segment_handler.h"
#include "events.h"

engine *init_engine(const char *wadPath, SDL_Renderer *renderer);

int update_engine(engine *e,int dt);

void engine_free(engine *e);
#endif