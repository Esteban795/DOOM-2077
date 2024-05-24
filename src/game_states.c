#include "../include/game_states.h"
#include <stdio.h>
//#include "../include/events.h"

bool firstTimeLaunching = true;

void switch_scene(engine *e, int scene) {
  game_states_free[e->state](e);
  e->state = scene;
  game_states_init[e->state](e);
}

void init_menu_state(engine *e) {
  SDL_ShowCursor(SDL_ENABLE);
  SDL_SetRelativeMouseMode(SDL_FALSE);
  e->substate = 0;
  e->uimodules = get_ui_menu(e->renderer, &e->nuimodules);
}
void init_ingame_state(engine *e) {
  SDL_ShowCursor(SDL_DISABLE);
  SDL_SetRelativeMouseMode(SDL_TRUE);
  e->substate = 0;
  e->uimodules = get_ui_ingame(e->renderer, &e->nuimodules);
}

void update_menu_state(engine *e) {
  
}

void update_ingame_state(engine *e) {
  if (keys[SDL_SCANCODE_TAB] && e->substate == SUBSTATE_INGAME_PLAYING){
    e->substate = SUBSTATE_INGAME_SCOREBOARD;
  } else if (!keys[SDL_SCANCODE_TAB] && e->substate == SUBSTATE_INGAME_SCOREBOARD) {
    e->substate = SUBSTATE_INGAME_PLAYING;
  }

  update_player(e->p);
  update_height(e->p);
  process_keys(e->p);
  position_ct *player_pos = player_get_position(e->p);
  vec2 pos2d = position_get_pos(player_pos);
  double angle = position_get_angle(player_pos);
  for (int i = 0; i < e->num_doors; i++) {
    door_update(e->doors[i], e->DT);
  }
  for (int i = 0; i < e->len_lifts; i++) {
    lift_update(e->lifts[i], e->DT);
  }
  segment_handler_update(e->seg_handler);
  update_bsp(e->bsp);
  // draw_crosshair(e->map_renderer,get_color(50,0),20);
  SDL_UpdateTexture(e->texture, NULL, e->pixels, WIDTH * 4);
  SDL_RenderCopy(e->renderer, e->texture, NULL, NULL);

  // enemies footsteps
  for (int i = 0; i < PLAYER_MAXIMUM; i++) {
    if (e->players[i] == NULL)
      continue;
    position_ct *enemy_pos = (position_ct *)world_get_component(
        e->world, e->players[i], COMPONENT_TAG_POSITION);
    if (enemy_pos->was_updated && position_has_moved(enemy_pos) &&
        position_get_walk_cooldown(enemy_pos) <=
            0) { // make sure enemy really was moved and that the sound cooldown
                 // is over
      vec2 enemy_pos2d = position_get_pos(enemy_pos);
      add_sound_to_play(TEMP_WALKING_SOUND, enemy_pos2d.x, enemy_pos2d.y);
      position_set_walk_cooldown(enemy_pos, WALK_SOUND_COOLDOWN);
    }
  }

  double volume, sound_angle;
  for (int i = 0; i < SOUNDS_INDEX; i++) {
    sound *s = get_sound_by_name(e->wData->sounds, e->wData->len_sounds,
                                 SOUNDS_TO_PLAY[i]->sound);
    if (s == NULL) {
      printf("Sound %s not found\n", SOUNDS_TO_PLAY[i]->sound);
      free(SOUNDS_TO_PLAY[i]);
      continue;
    }
    sound_angle = get_angular_distance(pos2d.x, pos2d.y, angle, SOUNDS_TO_PLAY[i]->x,
                                 SOUNDS_TO_PLAY[i]->y);
    volume = get_audio_gain(
        dist((vec2){SOUNDS_TO_PLAY[i]->x, SOUNDS_TO_PLAY[i]->y}, pos2d));
    audiomixer_play(e->mixer, s, sound_angle, volume);
    free(SOUNDS_TO_PLAY[i]);
  }
  SOUNDS_INDEX = 0;
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
