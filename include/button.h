#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WIDTH 600
#define HEIGHT 400

struct Button {
    int x;
    int y;
    int w;
    int h;
    bool pressed;
    void (*on_click)(void* data);
    char* text;
    size_t len_text;
    TTF_Font* font;
};

typedef struct Button button;
#endif