#include "../include/game_states.h"

bool firstTimeLaunching = true;

void switch_scene(engine *e, int scene) {
  game_states_free[e->state](e);
  e->state = scene;
  game_states_init[e->state](e);
}

void init_menu_state(engine *e) {
  e->substate = 0;
  e->uimodules = get_ui_menu(e->map_renderer->renderer, &e->nuimodules);
}
void init_ingame_state(engine *e) {
  e->substate = 0;
  e->uimodules = get_ui_ingame(e->map_renderer->renderer, &e->nuimodules);
}

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
  return;
}

void free_menu_state(engine *e) { free_ui(e->uimodules, e->nuimodules); }
void free_ingame_state(engine *e) { free_ui(e->uimodules, e->nuimodules); }

GameStateFunction game_states_init[] = {init_menu_state, init_ingame_state};

GameStateFunction game_states_update[] = {
    update_menu_state,
    update_ingame_state,
};

GameStateFunction game_states_free[] = {
    free_menu_state,
    free_ingame_state,
};
