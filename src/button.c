#include "../include//button.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

button *button_create(int x, int y, int w, int h,
                      void (*on_click)(void *data), char *text,TTF_Font* font) {
  button *b = malloc(sizeof(button));
  b->x = x;
  b->y = y;
  b->w = w;
  b->h = h;
  b->on_click = on_click;
  b->text = text;
  b->pressed = false;
  b->len_text = strlen(text);
  b->font = font;
  return b;
}

static void button_test_click(button *b, SDL_Event event, void *data) {
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    if (mouse_x > b->x + b->w || mouse_x < b->x || mouse_y > b->y + b->h ||
        mouse_y < b->y) {
      b->pressed = false;
      return;
    }
    b->pressed = true;
    b->on_click(data);
  }
}

// handles all kind of error at SDL startup
int start_SDL(SDL_Window **window, SDL_Renderer **renderer, int width,
              int height, const char *title) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    return 1;
  *window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       width, height, SDL_WINDOW_SHOWN);
  if (*window == NULL)
    return 1;
  *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
  if (*renderer == NULL)
    return 1;
  return 0;
}

static void draw_button(SDL_Renderer *r, button *btn) {
  // draw button
  SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
  SDL_Rect rect = {btn->x, btn->y, btn->w, btn->h};
  // if button press detected - reset it so it wouldn't trigger twice
  if (btn->pressed) {
    SDL_SetRenderDrawColor(r, 0, 255, 255, 255);
    btn->pressed = false;
  }
  SDL_RenderFillRect(r, &rect);
  SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
  SDL_Surface* text = TTF_RenderText_Solid(btn->font, btn->text, (SDL_Color){255, 0, 0, 255});
  SDL_Texture* texture = SDL_CreateTextureFromSurface(r, text);
  SDL_RenderCopy(r, texture, NULL, &rect);
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(text);
  SDL_RenderPresent(r);
}

void test_data(void *data) { printf("Button pressed\n"); }


void button_free(button* b){
  TTF_CloseFont(b->font);
  free(b);
}

int main(void) {
  if (TTF_Init() == -1){
    printf("TTF init error");
    exit(EXIT_FAILURE);
  }
  TTF_Font* font = TTF_OpenFont("./fonts/amazdoom/AmazDooMLeft.ttf", 28);
  if (font == NULL){
    printf("font init error");
    exit(EXIT_FAILURE);
  }
  SDL_Window *window;
  SDL_Renderer *renderer;
  int status = start_SDL(&window, &renderer, WIDTH, HEIGHT, "Map rendering..");
  if (status == 1) {
    printf("Error at SDL startup");
    exit(-1);
  }
  // SDL_Color white = {255,255,255,255};
  SDL_Event event;
  bool running = true;
  button *b = button_create(100, 100, 300, 150, test_data, "Button",font);
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
      button_test_click(b, event, NULL);
    }
    draw_button(renderer, b);
    SDL_RenderPresent(renderer);
  }
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  free(b);
  return 0;
}