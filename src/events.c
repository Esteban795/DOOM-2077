#include "../include/events.h"
#include <stdio.h>

uint16_t keys[SDL_NUM_SCANCODES] = {0};
int mouse[NUM_MOUSE_BUTTONS + 2] = {0}; // left, middle, right, mouse_motion_x, mouse_motion_y

void handle_events(engine *e) {
  SDL_Event event;
  SDL_Scancode scancode;
  int mouse_x, mouse_y;
  SDL_GetRelativeMouseState(&mouse_x, &mouse_y);
  mouse[NUM_MOUSE_BUTTONS] = mouse_x;
  mouse[NUM_MOUSE_BUTTONS + 1] = mouse_y;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      e->running = 0;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        e->running = 0;
      }
      scancode = event.key.keysym.scancode;
      keys[scancode] += e->DT;
      break;
    case SDL_KEYUP:
      scancode = event.key.keysym.scancode;
      keys[scancode] = 0;
      break;
    case SDL_MOUSEBUTTONDOWN:
      mouse[event.button.button - 1] = 1;
      break;
    case SDL_MOUSEBUTTONUP:
      mouse[event.button.button - 1] = 0;
      break;
    default:
      break;
    }
  }
}

bool is_key_held(SDL_Scancode scancode) {
  printf("Key %d is held for %d\n", scancode, keys[scancode]);
  return keys[scancode] > HOLD_DETECTION_DURATION;
}