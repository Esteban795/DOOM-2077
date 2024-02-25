#ifndef ENGINE_H
#define ENGINE_H

#include "bsp.h"
#include "map_renderer.h"
#include "player.h"
#include "structs.h"

engine *init_engine(const char *wadPath, SDL_Renderer *renderer, int numkeys, const uint8_t *keys);

int update_engine(engine *e);

void engine_free(engine *e);
#endif