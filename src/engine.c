#include "../include/engine.h"
#define num_players 1 //autres joueurs


engine *init_engine(const char *wadPath, SDL_Renderer *renderer, int numkeys,
                    const uint8_t *keys) {
  engine *e = malloc(sizeof(engine));
  e->wadPath = wadPath;
  e->running = true;
  e->wData = init_wad_data(e->wadPath);
  e->p = player_init(e);
  e->bsp = bsp_init(e, e->p);
  e->map_renderer = map_renderer_init(e, renderer);
  e->seg_handler = segment_handler_init(e);
  e->numkeys = numkeys;
  e->keys = keys;
  e->players = create_players(num_players,e);
  e->mixer = audiomixer_init();
  e->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                 SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
  return e;
}

int update_engine(engine *e, int dt) {
  e->DT = dt;
  SDL_PumpEvents(); // updates keys state
  if (e->keys[SDL_SCANCODE_ESCAPE]) {
    e->running = false;
    return 1;
  }
  if (e->keys[SDL_SCANCODE_SPACE]){
    fire_bullet(e->players,num_players,e->p,3);
  }
  int mouse_x, mouse_y;
  if(e->p->cooldown>1){
    e->p->cooldown=e->p->cooldown-1;
  }
  
  SDL_GetRelativeMouseState(&mouse_x, &mouse_y);
  SDL_SetRenderDrawColor(e->map_renderer->renderer, 0, 0, 0, 255);
  SDL_RenderClear(e->map_renderer->renderer);
  update_player(e->p, mouse_x, e->keys);
  get_ssector_height(e->bsp);
  segment_handler_update(e->seg_handler);
  update_bsp(e->bsp);
  audiomixer_update(e->mixer, dt);
  SDL_UpdateTexture(e->texture, NULL, e->pixels, WIDTH * 4);
  SDL_RenderCopy(e->map_renderer->renderer, e->texture, NULL, NULL);
  // draw_crosshair(e->map_renderer,get_color(50,0),20);
  SDL_RenderPresent(e->map_renderer->renderer);
  memset(e->pixels, 0, WIDTH * HEIGHT * 4);
  return 0;
}

void engine_free(engine *e) {
  wad_data_free(e->wData);
  bsp_free(e->bsp);
  player_free(e->p);
  map_renderer_free(e->map_renderer);
  segment_handler_free(e->seg_handler);
  players_free(e->players,num_players);
  audiomixer_free(e->mixer);
  free(e);
}
