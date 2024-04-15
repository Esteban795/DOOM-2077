#ifndef EVENTS_H
#define EVENTS_H
  
#include "stdlib.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

#define NUM_MOUSE_BUTTONS 3

extern uint8_t keys[SDL_NUM_SCANCODES];
extern int mouse[NUM_MOUSE_BUTTONS + 2];// to handle mouse buttons and events
extern bool running;

void handle_events();
#endif