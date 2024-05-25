#include "../include/engine.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>

#include "../include/ecs/entity.h"
#include "../include/ecs/world.h"
#include "../include/remote.h"
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
  e->state = STATE_MENU;
  e->substate = SUBSTATE_MENU_MAIN;
  e->uinextevent = 0;
  e->DT = 0;
  e->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_STREAMING, WIDTH,
                                 HEIGHT); // texture we will be rendering to
  e->remote = malloc(sizeof(remote_server_t));
  remote_init(e->remote, SERVER_ADDR, SERVER_PORT);
  e->wData = init_wad_data(wadPath);
  e->mixer = NULL;
  e->mixer = audiomixer_init();
  e->renderer = renderer;
  e->p = NULL;
  e->bsp = NULL;
  e->seg_handler = NULL;
  e->players = NULL;
  e->lifts = NULL;
  e->len_lifts = 0;
  e->doors = NULL;
  e->num_doors = 0;
  return e;
}

void read_map(engine *e, char *map_name) {
  load_map(e->wData, e->wadPath, map_name);
  e->world = malloc(sizeof(world_t));
  world_init(e->world);
  world_register_active_systems(e->world);
  e->p = player_init(e);
  e->bsp = bsp_init(e, e->p);
  e->seg_handler = segment_handler_init(e);
  e->players = calloc(PLAYER_MAXIMUM, sizeof(entity_t *));
  e->lifts = get_lifts(e->wData->linedefs, e->wData->len_linedefs,
                       &e->len_lifts, e->wData->sectors, e->wData->len_sectors);
  e->doors = get_doors(e->wData->linedefs, e->wData->len_linedefs,
                       &e->num_doors, e->wData->sectors, e->wData->len_sectors);
  game_states_init[e->state](e);
}

int update_engine(engine *e, int dt) {
  e->DT = dt;

  SDL_SetRenderDrawColor(e->renderer, 0, 0, 0, 255);
  SDL_RenderClear(e->renderer);

  // because MOVE packets aren't always updated, we need a boolean to make sure that 
  // each player actually got updated to make sure we play the walking sound
  // If not, there would be lingering sounds of players walking around, until they move
  // again
  for (int i = 0; i < PLAYER_MAXIMUM; i++) {
    if (e->players[i] == NULL)
      continue;
    position_ct *pos = (position_ct *)world_get_component(
        e->world, e->players[i], COMPONENT_TAG_POSITION);
    pos->was_updated = false;
    pos->walk_cooldown = max(-1, pos->walk_cooldown - dt);
  }

  remote_update(e, e->remote);
  if (world_queue_length(e->world) > 0) {
    // printf("Processing %d events...\n", world_queue_length(e->world));
    world_update(e->world);
  }
  // memset(e->pixels, 0, WIDTH * HEIGHT * sizeof(Uint32)); // resets the screen
  handle_events(e->DT); // process key presses and mouse movements
  game_states_update[e->state](e);
  for (int i = 0; i < e->nuimodules; i++) {
    update_uimodule(e->renderer, e->substate, e->uimodules[i],
                    &(e->uinextevent));
  }
  ui_handle_events(e);
  audiomixer_update(e->mixer, dt);
  // draw_crosshair(e->map_renderer,get_color(50,0),20);
  SDL_RenderPresent(e->renderer);
  return 0;
}

void engine_free(engine *e) {
  game_states_free[e->state](e);
  wad_data_free(e->wData);
  bsp_free(e->bsp);
  player_free(e->p);
  segment_handler_free(e->seg_handler);
  remote_destroy(e->remote);
  free(e->players);
  audiomixer_free(e->mixer);
  doors_free(e->doors, e->num_doors);
  lifts_free(e->lifts, e->len_lifts);
  SDL_DestroyTexture(e->texture);
  SDL_DestroyRenderer(e->renderer);
  world_destroy(e->world);
  free(e->world);
  free(e);
}
