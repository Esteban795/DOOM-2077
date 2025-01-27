#include "../../include/core/events.h"
#include <SDL2/SDL_events.h>

uint16_t keys[SDL_NUM_SCANCODES] = {0};
int mouse[NUM_MOUSE_BUTTONS + 4] = {
    0}; // left, right, middle, mouse_motion_x, mouse_motion_y, mx, my
char textinput[SDL_TEXTINPUTEVENT_TEXT_SIZE] = {'\0'};
bool running = 1;
bool is_controller = false;

void handle_joyaxismotion(SDL_JoyAxisEvent *event) {
  printf("Axis: %d Value: %d\n", event->axis, event->value);
}

void handle_joybuttondown(SDL_JoyButtonEvent *event) {
  printf("Button: %d\n", event->button);
}

void handle_joybuttonup(SDL_JoyButtonEvent *event) {
  printf("Button: %d\n", event->button);
}

void handle_controller(int DT) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_JOYAXISMOTION:
      handle_joyaxismotion(&event.jaxis);
      break;
    case SDL_JOYBUTTONDOWN:
      handle_joybuttondown(&event.jbutton);
      break;
    case SDL_JOYBUTTONUP:
      handle_joybuttonup(&event.jbutton);
      break;
    case SDL_JOYDEVICEREMOVED:
      printf("Controller removed\n");
      is_controller = false;
      break;
    default:
      break;
    }
  }
}

void handle_events(int DT) {
  if (is_controller) {
    handle_controller(DT);
    return;
  }
  SDL_Event event;
  SDL_Scancode scancode;
  int mouse_x, mouse_y;
  SDL_GetRelativeMouseState(&mouse_x, &mouse_y);
  mouse[NUM_MOUSE_BUTTONS] = mouse_x;
  mouse[NUM_MOUSE_BUTTONS + 1] = mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  mouse[NUM_MOUSE_BUTTONS + 2] = mouse_x;
  mouse[NUM_MOUSE_BUTTONS + 3] = mouse_y;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      running = 0;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        running = 0;
      }
      scancode = event.key.keysym.scancode;
      keys[scancode] += DT; // allows for keyholding detection
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
    case SDL_TEXTINPUT: {
      int i;
      for (i = 0; event.text.text[i] != '\0'; i++) {
        textinput[i] = event.text.text[i];
      }
      textinput[i + 1] = '\0';
      break;
    }
    case SDL_JOYDEVICEADDED:
      printf("Plugged in controller\n");
      is_controller = true;
      break;
    default:
      break;
    }
  }
}

bool is_key_held(SDL_Scancode scancode) {
  return keys[scancode] > HOLD_DETECTION_DURATION;
}
