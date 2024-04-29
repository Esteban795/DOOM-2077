#include "unistd.h"

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
    if (e->remote->connected == 1) {
      e->remote->connected = 2;
      break; // Connection established
    } else if (e->remote->connected == -2) {
      printf("Error at remote connection\n");
      break;
    } else if (e->remote->connected == -1) {
      break; // Solo mode
    }
    if (status < 0) {
      printf("Error while initializing the remote sync...");
    }
  }
  if (e->remote->connected < 2) {
    printf("Connection to server failed! Pursuing in solo...\n");
    e->remote->connected = -1;
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
    
    //printf("FPS: %f\n", 1000.0 / dt);
    old = now;
  }
  // Wait 100ms to be sure no other packet is sent during the same server tick.
  // Bug was: Segfault because we tried to access a component of a now removed entity
  // (for instance during a MOVE event)...
  // While the bug is fixed, it might reappear in the future (because C)! 
  // So just in case!
  usleep(100000);
  remote_disconnect(e->remote);
  engine_free(e);
  SDLNet_Quit();
  Mix_Quit();
  return 0;
}

