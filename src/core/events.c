#include "../../include/core/events.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_joystick.h>

uint16_t keys[SDL_NUM_SCANCODES] = {0};
int mouse[NUM_MOUSE_BUTTONS + 4] = {
    0}; // left, right, middle, mouse_motion_x, mouse_motion_y, mx, my
char textinput[SDL_TEXTINPUTEVENT_TEXT_SIZE] = {'\0'};
bool running = 1;
bool is_controller = false;
SDL_Joystick* controller = NULL;



void handle_events(int DT) {
  SDL_Event event;
  SDL_Scancode scancode;
  int mouse_x, mouse_y;
  SDL_GetRelativeMouseState(&mouse_x, &mouse_y);
  mouse[NUM_MOUSE_BUTTONS] = mouse_x;
  mouse[NUM_MOUSE_BUTTONS + 1] = mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  mouse[NUM_MOUSE_BUTTONS + 2] = mouse_x;
  mouse[NUM_MOUSE_BUTTONS + 3] = mouse_y;
  SDL_JoystickEventState(SDL_ENABLE);
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      running = 0;
      break;
    case SDL_JOYDEVICEADDED:
      printf("Plugged in controller\n");
      controller = SDL_JoystickOpen(0);
      printf("Controller name: %s\n", SDL_JoystickName(controller));
      
      break;
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
      SDL_JoystickClose(controller);
      controller = NULL;
      SDL_JoystickEventState(SDL_IGNORE);
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
    default:
      break;
    }
  }
}

bool is_key_held(SDL_Scancode scancode) {
  return keys[scancode] > HOLD_DETECTION_DURATION;
}
