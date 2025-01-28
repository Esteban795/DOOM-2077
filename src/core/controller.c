#include "../../include/core/controller.h"


void handle_joyaxismotion(SDL_JoyAxisEvent *event) {
    controls[AXIS_OFFSET + event->axis] = event->value;
}

void handle_joybuttondown(SDL_JoyButtonEvent *event) {
  controls[event->button] = 1;
}

void handle_joybuttonup(SDL_JoyButtonEvent *event) {
    controls[event->button] = 0;
}