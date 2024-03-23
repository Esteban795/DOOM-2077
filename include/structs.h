#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>

#include "keybindings.h"
#include "wad_data.h"
#include "vec2.h"
#include "settings.h"

struct Player;
struct BSP;
struct Engine;
struct MapRenderer;
struct SegmentHandler;

struct Player {
  struct Engine *engine;
  thing thing;
  vec2 pos;
  double angle;
  struct PlayerSetting *settings;
  struct PlayerKeybind *keybinds;
  double height;
  int life;
  int cooldown;
};

struct Engine {
  const char *wadPath;
  bool running;
  wad_data *wData;
  struct Player *p;
  struct BSP *bsp;
  struct MapRenderer *map_renderer;
  struct SegmentHandler *seg_handler;
  int numkeys;
  const uint8_t *keys;
  int DT;
  struct Player** players;
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

struct SegmentHandler {
  struct Engine *engine;
  player *player;
  segment *seg;
  double raw_angle_1;
  int screen_range[WIDTH];
  double upper_clip[WIDTH];
  double lower_clip[WIDTH];
  size_t screen_range_count;
};

typedef struct SegmentHandler segment_handler;
#endif
