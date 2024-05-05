#include "../include/engine.h"
#include <SDL2/SDL_render.h>
#include "../include/remote.h"
#include "../include/ecs/world.h"
#include "../include/ecs/entity.h"
#include "../include/settings.h"
#include "../include/system/client/active.h"

#ifndef SERVER_ADDR
#define SERVER_ADDR ""
#endif

#ifndef SERVER_PORT
#define SERVER_PORT 6942
#endif


engine *init_engine(const char *wadPath, SDL_Renderer *renderer) {
  engine *e = malloc(sizeof(engine));
  e->wadPath = wadPath;
  e->running = true;
  e->state = STATE_INGAME;
  game_states_init[e->state](e);
  e->DT = 0;
  e->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
  e->remote = malloc(sizeof(remote_server_t));
  remote_init(e->remote, SERVER_ADDR, SERVER_PORT);
  return e;
}

void read_map(engine *e, SDL_Renderer *renderer, char *map_name) {
  e->wData = init_wad_data(e->wadPath, map_name);
  e->world = malloc(sizeof(world_t));
  world_init(e->world);
  world_register_active_systems(e->world);
  e->p = player_init(e);
  e->bsp = bsp_init(e, e->p);
  e->map_renderer = map_renderer_init(e, renderer);
  e->seg_handler = segment_handler_init(e);
  e->players = calloc(PLAYER_MAXIMUM, sizeof(entity_t*));
  e->mixer = audiomixer_init();
}

int update_engine(engine *e, int dt) {
  e->DT = dt;
  SDL_SetRenderDrawColor(e->map_renderer->renderer, 0, 0, 0, 255);
  SDL_RenderClear(e->map_renderer->renderer);
  remote_update(e, e->remote);
  if (world_queue_length(e->world) > 0) {
    //printf("Processing %d events...\n", world_queue_length(e->world));
    world_update(e->world);
  }
  memset(e->pixels, 0, WIDTH * HEIGHT * 4);
  handle_events(e);
  game_states_update[e->state](e);
  audiomixer_update(e->mixer, dt);
  // draw_crosshair(e->map_renderer,get_color(50,0),20);
  SDL_RenderPresent(e->map_renderer->renderer);
  return 0;
}

void engine_free(engine *e) {
  game_states_free[e->state](e);
  wad_data_free(e->wData);
  bsp_free(e->bsp);
  player_free(e->p);
  map_renderer_free(e->map_renderer);
  segment_handler_free(e->seg_handler);
  remote_destroy(e->remote);
  free(e->players);
  audiomixer_free(e->mixer);
  world_destroy(e->world);
  free(e->world);
  free(e);
}
