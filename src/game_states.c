#include "../include/game_states.h"
#include <stdio.h>

uint64_t t0;
uint64_t time_elapsed_in_game;

bool firstTimeLaunching = true;
//weapons_array *wa;

void switch_scene(engine *e, int scene) {
  game_states_free[e->state](e);
  e->state = scene;
  game_states_init[e->state](e);
}

void init_menu_state(engine *e) {}

void init_ingame_state(engine *e) {
 // wa = init_weapons_array(e->map_renderer);
 t0 = SDL_GetTicks();
}

void update_menu_state(engine *e) {
  // faire les trucs du menu
}

void update_ingame_state(engine *e) {
  time_elapsed_in_game = SDL_GetTicks() - t0;
  update_player(e->p);
  for (int i = 0; i < e->num_doors; i++) {
    door_update(e->doors[i], e->DT);
  }
  for (int i = 0; i < e->len_lifts; i++) {
    lift_update(e->lifts[i], e->DT);
  }
  get_ssector_height(e->bsp);
  // update_players_subsectors(e->bsp);
  segment_handler_update(e->seg_handler);
  update_bsp(e->bsp);
  vssprite_sort(); // sorts the sprite by ascending scale
  render_vssprites(e);
  // draw_crosshair(e->map_renderer,get_color(50,0),20);
  SDL_UpdateTexture(e->texture, NULL, e->pixels, WIDTH * 4);
  
  SDL_SetTextureBlendMode(e->texture, SDL_BLENDMODE_BLEND);
  
  SDL_RenderCopy(e->renderer, e->texture, NULL, NULL);
  // update_weapons(e);
  return;
}

void free_menu_state(engine *e) {}
void free_ingame_state(engine *e) {}

GameStateFunction game_states_init[] = {init_menu_state, init_ingame_state};

GameStateFunction game_states_update[] = {
    update_menu_state,
    update_ingame_state,
};

GameStateFunction game_states_free[] = {
    free_menu_state,
    free_ingame_state,
};
