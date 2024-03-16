#ifndef TEXTAREA_H
#define TEXTAREA_H

#include <SDL2/SDL_rect.h>
#define BUFFER_SIZE 512

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define WIDTH 800
#define HEIGHT 600


#define TA_BG ((SDL_Color){45, 45, 45, 128})
#define TA_BORDER ((SDL_Color){255, 255, 255, 255})

struct TextArea {
    SDL_Rect rect;
    char text[BUFFER_SIZE];
    int text_index;
    bool is_active;
    TTF_Font *font;
};

typedef struct TextArea textarea;
#endif