#ifndef EVENTS_H
#define EVENTS_H

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>

#define HOLD_DETECTION_DURATION 250 //ms necessary to detect a key hold
#define NUM_MOUSE_BUTTONS 3

extern uint16_t keys[SDL_NUM_SCANCODES]; 
extern int mouse[NUM_MOUSE_BUTTONS + 4]; // to handle mouse buttons and events
extern char textinput[SDL_TEXTINPUTEVENT_TEXT_SIZE];
extern bool running;

void handle_events(int DT);

bool is_key_held(SDL_Scancode scancode);
#endif
