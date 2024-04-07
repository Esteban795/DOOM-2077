#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>

#include "keybindings.h"
#include "wad_data.h"
#include "vec2.h"
#include "settings.h"

#define STATE_COUNT 5

typedef enum {
  STATE_MENU,
  STATE_INGAME,
  STATE_PAUSE,
  STATE_GAMEOVER,
  STATE_SETTINGS
} GameState;

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
};

struct Engine {
  const char *wadPath;
  bool running;
  wad_data *wData;
  struct Player *p;
  struct BSP *bsp;
  struct MapRenderer *map_renderer;
  struct SegmentHandler *seg_handler;
  GameState state;
  int DT;
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
  int screen_range[WIDTH + 1];
  double upper_clip[WIDTH + 1];
  double lower_clip[WIDTH + 1];
  size_t screen_range_count;
};

typedef struct SegmentHandler segment_handler;

typedef struct {
  struct Engine* engine;
  bool *isFirstTime;
  bool isRunning;
} GameStateArgs;

#endif
