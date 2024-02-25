#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>

#include "wad_data.h"

struct Player;
struct BSP;
struct Engine;
struct MapRenderer;

struct Player {
  struct Engine *engine;
  thing thing;
  double x;
  double y;
  double angle;
};

struct Engine {
  const char *wadPath;
  bool running;
  wad_data *wData;
  struct Player *p;
  struct BSP *bsp;
  struct MapRenderer *map_renderer;
  int numkeys;
  const uint8_t* keys;
};

struct BSP {
  struct Engine *engine;
  struct Player *player;
  node *nodes;
  subsector *subsectors;
  segment *segments;
  size_t root_node_id;
};

struct MapRenderer {
  struct Engine *engine;
  SDL_Renderer *renderer;
  wad_data *wData;
  vertex *vertexes;
  linedef *linedefs;
  bbox map_bounds;
};

typedef struct Player player;
typedef struct Engine engine;
typedef struct BSP bsp;
typedef struct MapRenderer map_renderer;

struct Color {
  int r;
  int g;
  int b;
};

typedef struct Color color;

#endif
