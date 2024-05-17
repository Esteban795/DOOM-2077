#ifndef MAP_RENDERER_H
#define MAP_RENDERER_H

#include <SDL2/SDL.h>

#include "color.h"
#include "player.h"
#include "structs.h"
#include "vertex.h"
#include "settings.h"

void draw_crosshair(engine* e,color c,int size);

void draw_wall_column(engine* e, texture_map *texture,
                      double texture_column, int x, int y1, int y2,
                      double texture_alt, double inverted_scale,
                      i16 light_level);

void draw_flat(engine* e, flat *texture, i16 light_level, int x, int y1,
               int y2, int world_z);
#endif