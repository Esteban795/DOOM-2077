#ifndef UI_TEXTBOX_H
#define UI_TEXTBOX_H

#include "common.h"
#include "../events.h"

typedef struct _UITextBox {
  UICommon common;
  bool focused;
  SDL_Color bg_color;
  SDL_Color border_color;
  SDL_Color text_color;
  UIAnchorPoint anchor;
  char *placeholder;
  TTF_Font *font;
  char *text;
  int buffer_size;
  int text_index;
  SDL_Scancode scancode;
} UITextBox;

UITextBox *uitextbox_create(float x, float y, float w, float h,
                            UIAnchorPoint anchor, int *as, int nas,
                            TTF_Font *font, UIAnchorPoint text_anchor,
                            int buffer_size, SDL_Color bg, SDL_Color border,
                            SDL_Color text_color, SDL_Scancode scancode);

void uitextbox_free(UITextBox *tb);

void uitextbox_update(SDL_Renderer *r, int substate, UITextBox *tb);

void uitextbox_string_add(UITextBox *tb);

void uitextbox_char_remove(UITextBox *tb);

#endif
