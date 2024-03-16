#include "../include/textarea.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <stdio.h>

int min(int a, int b){
    return a < b ? a : b;
}

textarea* ta_create(int x, int y, int width, int height, TTF_Font* font){
    textarea* ta = malloc(sizeof(textarea));
    ta->rect = (SDL_Rect){x, y, width, height};
    ta->text_index = 0;
    ta->text[0] = '\0';
    ta->is_active = false;
    ta->font = font;
    return ta;
}

void ta_free(textarea* ta){
    free(ta);
}

void ta_add_char(textarea* ta, char c){
    if (ta->text_index == BUFFER_SIZE || !ta->is_active) return; //no more room
    ta->text[ta->text_index] = c;
    ta->text_index++;
    ta->text[ta->text_index] = '\0';
}

void ta_remove_char(textarea* ta){
    if (ta->text_index == 0) return; //no char
    ta->text_index--;
    ta->text[ta->text_index] = '\0';
}

void ta_clear(textarea* ta){
    ta->text_index = 0;
    ta->text[0] = '\0';
}

void ta_set_pos(textarea* ta, int x, int y){
    ta->rect.x = x;
    ta->rect.y = y;
}

void ta_set_size(textarea* ta, int width, int height){
    ta->rect.w = width;
    ta->rect.h = height;
}

void ta_render(SDL_Renderer* renderer, textarea* ta){
    if (!ta->is_active) return;
    SDL_SetRenderDrawColor(renderer, TA_BG.r, TA_BG.g, TA_BG.b, TA_BG.a);
    SDL_RenderFillRect(renderer, &ta->rect);
    SDL_SetRenderDrawColor(renderer, TA_BORDER.r, TA_BORDER.g, TA_BORDER.b, TA_BORDER.a);
    SDL_RenderDrawRect(renderer, &ta->rect);
    if (ta->text_index > 0){
        SDL_RenderSetClipRect(renderer, &ta->rect);
        int w, h;
        TTF_SizeText(ta->font, ta->text, &w, &h);
        SDL_Surface* surface = TTF_RenderUTF8_Solid(ta->font, ta->text, TA_BORDER);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        int x_offset = 0;
        if (w > ta->rect.w - 10){
            x_offset = w - ta->rect.w + 10;
        }
        SDL_Rect vertical = {ta->rect.x + 5 - x_offset, ta->rect.y + (h / 2), w, h};
        SDL_Rect cursor = (SDL_Rect){ta->rect.x + w - x_offset + 5, ta->rect.y + 5, 2, ta->rect.h - 10};
        SDL_RenderFillRect(renderer, &cursor);
        SDL_RenderCopy(renderer, texture, NULL, &vertical);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    } else {
        SDL_Rect cursor = {ta->rect.x + 5, ta->rect.y + 5, 2, ta->rect.h - 10};
        SDL_RenderFillRect(renderer, &cursor);
    }
}



// EXAMPLE OF HOW IT CAN BE USED

// int start_SDL(SDL_Window **window, SDL_Renderer **renderer, int width,
//               int height, const char *title) {
//   if (SDL_Init(SDL_INIT_VIDEO) != 0)
//     return 1;
//   *window =
//       SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//                        width, height, SDL_WINDOW_SHOWN);
//   if (*window == NULL)
//     return 1;
//   *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
//   if (*renderer == NULL)
//     return 1;
//   return 0;
// }


// int main(void) {
//   if (TTF_Init() == -1) {
//     printf("TTF init error");
//     exit(EXIT_FAILURE);
//   }
//   TTF_Font *font = TTF_OpenFont("./fonts/lemon_milk/LEMONMILK-Bold.otf", 20);
//   if (font == NULL) {
//     printf("font init error");
//     exit(EXIT_FAILURE);
//   }
//   SDL_Window *window;
//   SDL_Renderer *renderer;
//   int status = start_SDL(&window, &renderer, WIDTH, HEIGHT, "Map rendering..");
//   if (status == 1) {
//     printf("Error at SDL startup");
//     exit(-1);
//   }
//   SDL_Event event;
//   bool running = true;
//   textarea* ta = ta_create(100, 100, 300, 40,font);
//   while (running) {
//     while (SDL_PollEvent(&event)) {
//       if (event.type == SDL_QUIT) {
//         running = false;
//       } else if (event.type == SDL_KEYDOWN) {
//         if (event.key.keysym.sym == SDLK_BACKSPACE) {
//           ta_remove_char(ta);
//         } else if (event.key.keysym.sym == SDLK_RETURN) {
//           ta_clear(ta);
//         } else if (event.key.keysym.sym == SDLK_ESCAPE) {
//           ta->is_active = true;
//         } else if (event.type == SDL_TEXTINPUT) {
//             ta_add_char(ta, event.text.text[0]);
//             printf("%s\n", ta->text);
//         }
//       } else if (event.type == SDL_TEXTINPUT) {
//         ta_add_char(ta, event.text.text[0]);
//       }
//     }
//     ta_render(renderer, ta);
//     SDL_RenderPresent(renderer);
//   }
//   TTF_CloseFont(font);
//   TTF_Quit();
//   SDL_DestroyRenderer(renderer);
//   SDL_DestroyWindow(window);
//   SDL_Quit();
//   ta_free(ta);
//   return 0;
// }
