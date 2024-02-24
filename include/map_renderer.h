#ifndef MAP_RENDERER_H
#define MAP_RENDERER_H

#include <SDL2/SDL.h>

#include "player.h"
#include "settings.h"
#include "structs.h"
#include "vertex.h"

int *get_map_bounds(vertex *vertexes, int len);

int remap_x(int current_x, int x_min, int x_max);

int remap_y(int current_y, int y_min, int y_max);

vertex *remap_vertexes(vertex *vertexes, int len, int *map_bounds);

map_renderer *map_renderer_init(engine *e, SDL_Renderer *renderer);

void draw_subsector(map_renderer *mr, i16 subsector_id);

void draw_node(map_renderer *mr, int node_id);

void draw_segment(map_renderer *mr, segment seg);

void draw_linedefs(SDL_Renderer *renderer, linedef *linedefs, int len);

void draw(map_renderer *mr);

void draw_vertical_lines(map_renderer *mr, int x1, int x2, i16 subsector_id);

void map_renderer_free(map_renderer *mr);
#endif