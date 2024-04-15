#ifndef UI_TEXTBOX_H
#define UI_TEXTBOX_H

#include "common.h"
#include <SDL2/SDL_ttf.h>

typedef struct _UITextBox {
  UICommon common;
  bool focused;
  SDL_Color bg_color;
  SDL_Color border_color;
  char *placeholder;
  TTF_Font *font;
  char *text;
  int buffer_size;
  int text_index;
} UITextBox;

UITextBox *uitextbox_create(float x, float y, float w, float h,
                            UIAnchorPoint anchor, TTF_Font *font,
                            int buffer_size, SDL_Color bg, SDL_Color border,
                            char *placeholder);

void uitextbox_free(UITextBox *tb);

void uitextbox_update(SDL_Renderer *r, UITextBox *tb);

#endif
