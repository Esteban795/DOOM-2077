#include "../include/engine.h"
#include "../include/weapons.h"

weapons_array *wa;

// handles all kind of error at SDL startup
int start_SDL(SDL_Window **window, SDL_Renderer **renderer, int width,
              int height, const char *title) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    return 1;
  *window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_SHOWN);
  if (*window == NULL)
    return 1;
  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
  if (*renderer == NULL)
    return 1;
  return 0;
}

int main() {
  SDL_Window *window;
  SDL_Renderer *renderer;
  int status = start_SDL(&window, &renderer, WIDTH, HEIGHT, "Map rendering..");
  if (status == 1) {
    printf("Error at SDL startup\n");
    exit(-1);
  }
  status = Mix_Init(MIX_INIT_MOD);
  if (status == 1) {
    printf("Error at Mix startup\n");
    exit(-1);
  }
  
  uint64_t now;
  uint64_t old = SDL_GetTicks();
  SDL_ShowCursor(SDL_DISABLE);
  SDL_SetRelativeMouseMode(SDL_TRUE);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  engine *e = init_engine("maps/DOOM1.WAD",renderer);
  read_map(e, renderer, "E1M2");
  for(int i=1; i<WEAPONS_NUMBER; i++){
      add_weapon(e->p, i, wa);
  }
  //print_animations_patches(wa->weapons[1]);
  int dt = 0;
  while (e->running) {
    now = SDL_GetTicks();
    dt = now - old;
    int res = update_engine(e, dt);

    if (res == 1)
      break;


    aux_fire_bullet(e->map_renderer,wa);
    //printf("%f\n",e->p->spray);
    //printf("FPS: %f\n",1000/dt);
    old = now;
    // break;
  }
  free_weapons_array(wa);
  engine_free(e);
  Mix_Quit();
  return 0;
}

