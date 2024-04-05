#include "../include/events.h"

uint8_t keys[SDL_NUM_SCANCODES] = {0};
uint8_t mouse[NUM_MOUSE_BUTTONS + 2] = {0};

void handle_events(engine *e) {
  SDL_Event event;
  SDL_Scancode scancode;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      e->running = 0;
      break;
    case SDL_KEYDOWN:
      scancode = event.key.keysym.scancode;
      keys[scancode] = 1;
      break;
    case SDL_KEYUP:
      scancode = event.key.keysym.scancode;
      keys[scancode] = 0;
      break;
    case SDL_MOUSEBUTTONDOWN:
      mouse[event.button.button] = 1;
      break;
    case SDL_MOUSEBUTTONUP:
      mouse[event.button.button] = 0;
      break;
    case SDL_MOUSEMOTION:
      mouse[NUM_MOUSE_BUTTONS] = event.motion.x;
      mouse[NUM_MOUSE_BUTTONS + 1] = event.motion.y;
      break;
    default:
      break;
    }
    game_states[e->state](NULL);
  }
  
}