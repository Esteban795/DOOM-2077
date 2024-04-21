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

int main() {
  SDL_Window *window;
  SDL_Renderer *renderer;
  int status = start_SDL(&window, &renderer, WIDTH, HEIGHT, "Map rendering..");
  if (status == 1) {
    printf("Error at SDL startup\n");
    exit(-1);
  }
  status = SDLNet_Init();
  if (status == -1) {
    printf("Error at SDLNet startup");
    exit(-2);
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
  engine *e = init_engine("maps/DOOM1.WAD",renderer);
  
  // Waiting for connection to server
  old = SDL_GetTicks();
  while(SDL_GetTicks() - old < 5000) {
    int status = remote_update(e, e->remote);
    if (status == 0)
      break; // Connection established
    if (status == -1) {
      printf("Network error occured! Pursuing in solo...\n");
    } else {
      printf("Server refused the connection! Pursuing in solo...\n");
    }
    e->remote->player_id = 0;
  }
  read_map(e, renderer, "E1M2");
  int dt = 0;
  while (e->running) {
    now = SDL_GetTicks();
    dt = now - old;
    int res = update_engine(e, dt);
    if (res == 1)
      break;
    
    printf("FPS: %f\n", 1000.0 / dt);
    old = now;
  }
  engine_free(e);
  SDLNet_Quit();
  Mix_Quit();
  return 0;
}

