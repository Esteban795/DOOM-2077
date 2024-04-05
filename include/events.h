#ifndef EVENTS_H
#define EVENTS_H

#include "structs.h"    
#include "game_states.h"

#define NUM_MOUSE_BUTTONS 3

extern uint8_t keys[SDL_NUM_SCANCODES];
extern uint8_t mouse[NUM_MOUSE_BUTTONS + 2];// to handle mouse buttons and events

#endif