#include "../include/game_states.h"
#include <stdio.h>

bool firstTimeLaunching = true;

void switch_scene(engine *e, int scene) {
  game_states_free[e->state](e);
  e->state = scene;
  game_states_init[e->state](e);
}

void init_menu_state(engine *e) {}
void init_ingame_state(engine *e) {}

void update_menu_state(engine *e) {
  // faire les trucs du menu
}

void update_ingame_state(engine *e) {
  update_player(e->p);
  get_ssector_height(e->bsp);
  segment_handler_update(e->seg_handler);
  update_bsp(e->bsp);
  // draw_crosshair(e->map_renderer,get_color(50,0),20);
  SDL_UpdateTexture(e->texture, NULL, e->pixels, WIDTH * 4);
  SDL_RenderCopy(e->map_renderer->renderer, e->texture, NULL, NULL);
  patch *p =
      get_patch_from_name(e->wData->sprites, e->wData->len_sprites, "SARGE1");
  if (p != NULL) {
    render_sprite(e->map_renderer, p, 0, 0);
    render_sprite(e->map_renderer, p, 448,128);
    printf("\n");
  }
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
