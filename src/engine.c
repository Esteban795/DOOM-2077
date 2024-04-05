#include "../include/engine.h"
#include <SDL2/SDL_timer.h>

engine *init_engine(const char *wadPath, SDL_Renderer *renderer) {
  engine *e = malloc(sizeof(engine));
  e->wadPath = wadPath;
  e->running = true;
  e->wData = init_wad_data(e->wadPath);
  e->p = player_init(e);
  e->bsp = bsp_init(e, e->p);
  e->map_renderer = map_renderer_init(e, renderer);
  e->seg_handler = segment_handler_init(e);
  return e;
}

int update_engine(engine *e, int dt) {
  e->DT = dt;
  SDL_SetRenderDrawColor(e->map_renderer->renderer, 0, 0, 0, 255);
  SDL_RenderClear(e->map_renderer->renderer);
  handle_events(e);
  update_player(e->p);
  get_ssector_height(e->bsp);
  segment_handler_update(e->seg_handler);
  update_bsp(e->bsp);
  // SDL_Delay(500);
  SDL_RenderPresent(e->map_renderer->renderer);
  return 0;
}

void engine_free(engine *e) {
  wad_data_free(e->wData);
  bsp_free(e->bsp);
  player_free(e->p);
  map_renderer_free(e->map_renderer);
  segment_handler_free(e->seg_handler);
  free(e);
}