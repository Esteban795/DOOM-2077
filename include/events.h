#ifndef EVENTS_H
#define EVENTS_H

#include "structs.h"    

#define HOLD_DETECTION_DURATION 250 //ms necessary to detect a key hold
#define NUM_MOUSE_BUTTONS 3

extern uint16_t keys[SDL_NUM_SCANCODES];
extern int mouse[NUM_MOUSE_BUTTONS + 2];// to handle mouse buttons and events

void handle_events(engine *e);

bool is_key_held(SDL_Scancode scancode);
#endif