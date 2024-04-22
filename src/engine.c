#include "../include/engine.h"
#define num_players 1 // autres joueurs

engine *init_engine(const char *wadPath, SDL_Renderer *renderer) {
  engine *e = malloc(sizeof(engine));
  e->wadPath = wadPath;
  e->state = STATE_INGAME;
  game_states_init[e->state](e);
  e->DT = 0;
  e->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
  return e;
}

void read_map(engine *e, SDL_Renderer *renderer, char *map_name) {
  e->wData = init_wad_data(e->wadPath, map_name);
  e->p = player_init(e);
  e->bsp = bsp_init(e, e->p);
  e->map_renderer = map_renderer_init(e, renderer);
  e->seg_handler = segment_handler_init(e);
  e->uimodules = get_ui_ingame(renderer, &e->nuimodules);
  e->players = create_players(num_players, e);
  e->mixer = audiomixer_init();
}

int update_engine(engine *e, int dt) {
  e->DT = dt;

  SDL_SetRenderDrawColor(e->map_renderer->renderer, 0, 0, 0, 255);
  SDL_RenderClear(e->map_renderer->renderer);
  handle_events(e);
  game_states_update[e->state](e);
  audiomixer_update(e->mixer, dt);
  SDL_UpdateTexture(e->texture, NULL, e->pixels, WIDTH * 4);
  SDL_RenderCopy(e->map_renderer->renderer, e->texture, NULL, NULL);
  // draw_crosshair(e->map_renderer,get_color(50,0),20);
  SDL_RenderPresent(e->map_renderer->renderer);
  memset(e->pixels, 0, WIDTH * HEIGHT * 4);
  return 0;
}

void engine_free(engine *e) {
  game_states_free[e->state](e);
  wad_data_free(e->wData);
  bsp_free(e->bsp);
  player_free(e->p);
  map_renderer_free(e->map_renderer);
  segment_handler_free(e->seg_handler);
  players_free(e->players, num_players);
  audiomixer_free(e->mixer);
  free(e);
}
