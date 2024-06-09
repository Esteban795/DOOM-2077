#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "../core/events.h"
#include "common.h"

#include <stdlib.h>

typedef struct _UIButton {
  UICommon common;
  UIAnchorPoint text_anchor;
  bool hovered;
  bool pressed;
  SDL_Color bg_color;
  SDL_Color border_color;
  SDL_Color bg_color_pressed;     // pointers to make them optional
  SDL_Color border_color_pressed; // ^
  int assigned_event;
  SDL_Scancode scancode;
} UIButton;

UIButton *uibutton_create(float x, float y, float w, float h,
                          UIAnchorPoint uianchor, int *as, int nas,
                          SDL_Color bg_color, SDL_Color border_color,
                          SDL_Color bg_color_pressed,
                          SDL_Color border_color_pressed, int ae, SDL_Scancode sc);

void uibutton_free(UIButton *uibutton);

void uibutton_update(SDL_Renderer *r, int substate, UIButton *uibutton,
                     int *uinextevent);

#endif
