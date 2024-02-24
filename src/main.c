#include "../include/engine.h"

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

int main(void) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  int numkeys;
  const uint8_t *keys = SDL_GetKeyboardState(&numkeys);
  int status = start_SDL(&window, &renderer, WIDTH, HEIGHT, "Map rendering..");
  if (status == 1) {
    printf("Error at SDL startup");
    exit(-1);
  }
  uint64_t now;
  uint64_t old = SDL_GetTicks64();
  engine *e = init_engine("maps/DOOM1.WAD", renderer, numkeys, keys);
  while (e->running) {
    now = SDL_GetTicks64();
    int res = update_engine(e);
    if (res == 1)
      break;
    printf("FPS: %f\n", 1000.0 / (now - old));
    old = now;
  }
  engine_free(e);
  return 0;
}