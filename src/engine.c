#include "../include/engine.h"
#include <SDL2/SDL_mouse.h>
#include <stdbool.h>

engine *init_engine(const char *wadPath, SDL_Renderer *renderer, int numkeys,
                    const uint8_t *keys) {
  engine *e = malloc(sizeof(engine));
  e->wadPath = wadPath;
  e->running = true;
  e->wData = init_wad_data(e->wadPath);
  e->p = player_init(e);
  e->bsp = bsp_init(e, e->p);
  e->map_renderer = map_renderer_init(e, renderer);
  e->numkeys = numkeys;
  e->keys = keys;
  return e;
}

int update_engine(engine *e) {
  SDL_PumpEvents(); // updates keys state
  if (e->keys[SDL_SCANCODE_ESCAPE]) {
    e->running = false;
    return 1;
  }
  int mouse_x, mouse_y;
  SDL_GetRelativeMouseState(&mouse_x, &mouse_y);
  SDL_SetRenderDrawColor(e->map_renderer->renderer, 0, 0, 0, 255);
  SDL_RenderClear(e->map_renderer->renderer);
  draw_linedefs(
      e->map_renderer->renderer, e->wData->linedefs, e->wData->len_linedefs,
      e->map_renderer
          ->vertexes); // to make it visible what we are actually seeing
  update_player(e->p, mouse_x, e->keys);
  update_bsp(e->bsp);
  draw(e->map_renderer);
  SDL_RenderPresent(e->map_renderer->renderer);
  return 0;
}

void engine_free(engine *e) {
  wad_data_free(e->wData);
  bsp_free(e->bsp);
  player_free(e->p);
  map_renderer_free(e->map_renderer);
  free(e);
}