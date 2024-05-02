#ifndef MAP_RENDERER_H
#define MAP_RENDERER_H

#include <SDL2/SDL.h>

#include "color.h"
#include "player.h"
#include "structs.h"
#include "vertex.h"
#include "settings.h"
#include "drawseg.h"
#include "vssprite.h"

extern player* players_to_draw[NUM_PLAYERS];
extern int next_index;

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

void map_renderer_free(map_renderer *mr);

void draw_vline(map_renderer *mr, int x, int y1, int y2, color c);

void draw_crosshair(map_renderer *mr,color c,int size);

void draw_wall_column(map_renderer *mr, texture_map *texture,
                      double texture_column, int x, int y1, int y2,
                      double texture_alt, double inverted_scale, i16 light_level);

void draw_flat(map_renderer *mr, flat *texture, i16 light_level, int x,
               int y1, int y2, int world_z);

void render_vssprites(map_renderer* mr);
#endif