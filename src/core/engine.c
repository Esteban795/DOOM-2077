#include "../../include/core/engine.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>

#include "../../include/component/animation.h"
#include "../../include/ecs/entity.h"
#include "../../include/ecs/world.h"
#include "../../include/remote.h"
#include "../../include/settings.h"
#include "../../include/shared.h"
#include "../../include/system/client/active.h"

engine *init_engine(const char *wadPath, SDL_Renderer *renderer) {
  engine *e = SHARED_ENGINE;
  e->wadPath = wadPath;
  e->state = STATE_MENU;
  e->substate = SUBSTATE_MENU_MAIN;
  e->uinextevent = 0;
  e->DT = 0;
  e->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_STREAMING, WIDTH,
                                 HEIGHT); // texture we will be rendering to
  e->remote = calloc(1, sizeof(remote_server_t));
  e->wData = init_wad_data(renderer, wadPath);
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
  strncpy(e->player_name, "Player", 127);

  wa = init_weapons_array(e);
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

void engine_reset(engine *e) {
  free_map(e->wData);
  bsp_free(e->bsp);
  player_free(e->p);
  segment_handler_free(e->seg_handler);
  free(e->players);
  lifts_free(e->lifts, e->len_lifts);
  doors_free(e->doors, e->num_doors);
  remote_destroy(e->remote);
  world_destroy(e->world);
  free(e->world);
  e->p = NULL;
  e->bsp = NULL;
  e->seg_handler = NULL;
  e->players = NULL;
  e->lifts = NULL;
  e->len_lifts = 0;
  e->doors = NULL;
  e->num_doors = 0;
  e->world = NULL;
  e->remote = calloc(1, sizeof(remote_server_t));
}

int update_engine(engine *e, int dt) {
  e->DT = dt;
  fmt = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  SDL_SetRenderDrawColor(e->renderer, 0, 0, 0, 255);
  SDL_RenderClear(e->renderer);

  // because MOVE packets aren't always updated, we need a boolean to make sure
  // that each player actually got updated to make sure we play the walking
  // sound If not, there would be lingering sounds of players walking around,
  // until they move again
  for (int i = 0; i < PLAYER_MAXIMUM; i++) {
    if (e->players[i] == NULL)
      continue;
    position_ct *pos = (position_ct *)world_get_component(
        e->world, e->players[i], COMPONENT_TAG_POSITION);
    animation_ct *animation = (animation_ct *)world_get_component(
        e->world, e->players[i], COMPONENT_TAG_ANIMATION);
    pos->was_updated = false;
    pos->walk_cooldown = max(-1, pos->walk_cooldown - dt);
    animation->shot_filter_duration =
        max(-1, animation->shot_filter_duration - dt);
  }

  remote_update(e, e->remote);
  if (world_queue_length(e->world) > 0) {
    // printf("Processing %d events...\n", world_queue_length(e->world));
    world_update(e->world);
  }
  for (int i = 0; i < PLAYER_MAXIMUM; i++) {
    if (e->players[i] == NULL)
      continue;
    ANIMATION_COOLDOWNS[i] -= dt;
  }
  memset(e->pixels, 0, WIDTH * HEIGHT * sizeof(Uint32)); // resets the screen
  handle_events(e->DT); // process key presses and mouse movements
  game_states_update[e->state](e);
  for (int i = 0; i < e->nuimodules; i++) {
    update_uimodule(e->renderer, e->substate, e->uimodules[i],
                    &(e->uinextevent), e->DT);
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
}
