#ifndef TEXTAREA_H
#define TEXTAREA_H

#define BUFFER_SIZE 512

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define TA_BG ((SDL_Color){45, 45, 45, 128})

extern char buffer[BUFFER_SIZE];

struct TextArea {
    int x;
    int y;
    int width;
    int height;
    char text[BUFFER_SIZE];
    int text_index;
};

typedef struct TextArea textarea;
#endif