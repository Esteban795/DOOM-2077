#include "../include/engine.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include "../include/game_states.h"

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
  const uint8_t* keys = SDL_GetKeyboardState(&numkeys);
  int status = start_SDL(&window, &renderer, WIDTH, HEIGHT, "Map rendering..");
  if (status == 1) {
    printf("Error at SDL startup");
    exit(-1);
  }
  uint64_t now;
  uint64_t old = SDL_GetTicks64();
  SDL_ShowCursor(SDL_DISABLE);
  bool isFirstTime[STATE_COUNT];
  for(int i=0; i<STATE_COUNT; i++) {
    isFirstTime[i] = true;
  }
  GameState current_state = STATE_INGAME;
  GameStateArgs args;
  args.isRunning = true;
  args.isFirstTime = isFirstTime;
  args.renderer = renderer;
  args.numkeys = numkeys;
  args.keys = keys;
  int dt = 0;
  while (args.isRunning) {
    game_states[current_state](&args);
    now = SDL_GetTicks64();
    dt = now - old;
    int res = update_engine(e, dt);
    if (res == 1)
      break;
    printf("FPS: %f\n", 1000.0 / dt);
    old = now;
  }
  if(args.e != NULL) {
    engine_free(args.e);
  }
  return 0;
}