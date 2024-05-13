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
  for (int i = 0; i < e->num_doors; i++) {
    door_update(e->doors[i], e->p->pos,e->DT);
  }
  for (int i = 0; i < e->len_lifts; i++) {
    lift_update(e->lifts[i], e->p->pos,e->DT);
  }
  get_ssector_height(e->bsp);
  segment_handler_update(e->seg_handler);
  update_bsp(e->bsp);
  // draw_crosshair(e->map_renderer,get_color(50,0),20);
  SDL_UpdateTexture(e->texture, NULL, e->pixels, WIDTH * 4);
  SDL_RenderCopy(e->map_renderer->renderer, e->texture, NULL, NULL);
  for (int i = 0; i < SOUNDS_INDEX;i++){
    sound* s = get_sound_by_name(e->wData->sounds, e->wData->len_sounds, SOUNDS_TO_PLAY[i].sound);
    printf("sound %s\n",s->name);
    printf("angle %f\n",SOUNDS_TO_PLAY[i].angle);
    printf("volume %f\n",SOUNDS_TO_PLAY[i].volume);
    audiomixer_play(e->mixer, s, deg_to_rad(SOUNDS_TO_PLAY[i].angle) , SOUNDS_TO_PLAY[i].volume);
  }
  SOUNDS_INDEX = 0;
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
