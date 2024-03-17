#ifndef BUTTON_H
#define BUTTON_H

#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include "color.h"

struct Button {
  int x;
  int y;
  int w;
  int h;
  bool pressed;
  bool hovered;
  bool text_centered;
  void (*on_click)(void *data);
  char *text;
  size_t len_text;
  TTF_Font *font;
  color bg_color;
  color font_color;
};

typedef struct Button button;

button *button_create(int x, int y, int w, int h, void (*on_click)(void *data),
                      bool text_centered, char *text, TTF_Font *font,
                      color bg_c, color font_c);

button *button_copy(button *b);

void button_free(button *b);

void button_detect_click(button *b);

void button_render(SDL_Renderer *r, button *btn);

void button_set_text(button *b, char *text);

void button_set_font(button *b, TTF_Font *font);

void button_set_coords(button *b, int x, int y);

void button_set_size(button *b, int w, int h);

void button_set_callback(button *b, void (*on_click)(void *data));

void button_set_bg_color(button *b, color c);

void button_set_font_color(button *b, color c);

bool button_pressed(button *b);
#endif