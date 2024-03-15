#include "../include/textarea.h"

textarea* ta_create(int x, int y, int width, int height){
    textarea* ta = malloc(sizeof(textarea));
    ta->x = x;
    ta->y = y;
    ta->width = width;
    ta->height = height;
    return ta;
}

void ta_free(textarea* ta){
    free(ta);
}

void ta_add_char(textarea* ta, char c){
    if (ta->text_index == BUFFER_SIZE) return; //no more room
    ta->text[ta->text_index] = c;
}

void ta_remove_char(textarea* ta){
    if (ta->text_index == 0) return; //no char
    ta->text[ta->text_index] = '\0';
}

void ta_clear(textarea* ta){
    ta->text_index = 0;
    ta->text[0] = '\0';
}

void ta_set_pos(textarea* ta, int x, int y){
    ta->x = x;
    ta->y = y;
}

void ta_set_size(textarea* ta, int width, int height){
    ta->width = width;
    ta->height = height;
}

void ta_render(SDL_Renderer* renderer, textarea* ta){
    SDL_Rect ta_rect = (SDL_Rect){ta->x, ta->y, ta->width, ta->height};
    SDL_SetRenderDrawColor(renderer, TA_BG.r, TA_BG.g, TA_BG.b, TA_BG.a);
    
}

void ta_draw(textarea* ta){
    SDL_Rect ta_rect = (SDL_Rect){ta->x, ta->y, ta->width, ta->height};
}