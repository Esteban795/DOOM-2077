#include "../include//button.h"

button *button_create(int x, int y, int w, int h, void (*on_click)(void *data),
                      bool text_centered, char *text, TTF_Font *font,
                      color bg_c, color font_c) {
  button *b = malloc(sizeof(button));
  b->x = x;
  b->y = y;
  b->w = w;
  b->h = h;
  b->on_click = on_click;
  b->text = text;
  b->pressed = false;
  b->hovered = false;
  b->text_centered = text_centered;
  b->len_text = strlen(text);
  b->font = font;
  b->bg_color = bg_c;
  b->font_color = font_c;
  return b;
}

button *button_copy(button *b) {
  button *new = malloc(sizeof(button));
  new->x = b->x;
  new->y = b->y;
  new->w = b->w;
  new->h = b->h;
  new->on_click = b->on_click;
  new->text = b->text;
  new->pressed = b->pressed;
  new->hovered = b->hovered;
  new->text_centered = b->text_centered;
  new->len_text = b->len_text;
  new->font = b->font;
  new->bg_color = b->bg_color;
  new->font_color = b->font_color;
  return new;
}

void button_free(button *b) { free(b); }

void button_detect_click(button *b, void *data) {
  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  if (mouse_x > b->x + b->w || mouse_x < b->x || mouse_y > b->y + b->h ||
      mouse_y < b->y) {
    b->pressed = false;
    return;
  }
  b->pressed = true;
}

void button_render(SDL_Renderer *r, button *btn) {
  SDL_SetRenderDrawColor(r, btn->bg_color.r, btn->bg_color.g, btn->bg_color.b,
                         btn->bg_color.a);
  SDL_Rect rect = {btn->x, btn->y, btn->w, btn->h};
  // if button press detected - reset it so it doesn't trigger twice
  if (btn->pressed) {
    btn->pressed = false;
  }
  SDL_Color fc = {255, 255, 255, 255};
  SDL_RenderFillRect(r, &rect);
  SDL_Surface *text = TTF_RenderText_Blended(btn->font, btn->text, fc);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(r, text);
  SDL_Rect dstrect = {btn->x, btn->y, btn->w, btn->h};
  if (btn->text_centered) {
    int w, h;
    TTF_SizeText(btn->font, btn->text, &w, &h);
    dstrect = (SDL_Rect){btn->x + btn->w / 2 - w / 2,
                         btn->y + btn->h / 2 - h / 2, w, h};
  }
  SDL_RenderCopy(r, texture, NULL, &dstrect);
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(text);
}

void button_set_text(button *b, char *text) {
  b->text = text;
  b->len_text = strlen(text);
}

void button_set_font(button *b, TTF_Font *font) { b->font = font; }

void button_set_coordinates(button *b, int x, int y) {
  b->x = x;
  b->y = y;
}

void button_set_size(button *b, int w, int h) {
  b->w = w;
  b->h = h;
}

void button_set_callback(button *b, void (*on_click)(void *data)) {
  b->on_click = on_click;
}

void button_set_bg_color(button *b, color c) { b->bg_color = c; }

void button_set_font_color(button *b, color c) { b->font_color = c; }

bool button_pressed(button *b) { return b->pressed; }