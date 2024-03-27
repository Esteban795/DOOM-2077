#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "common.h"

#include <stdlib.h>

typedef struct _UIButton {
    UICommon common;
    UIAnchorPoint text_anchor;
    bool hovered = false;
    bool pressed = false;
    SDL_Color bg_color;
    SDL_Color border_color;
    SDL_Color* bg_color_hovered; // pointers to make them optional
    SDL_Color* border_color_hovered; // ^
    void (*on_click)(void* data);
} UIButton;

UIButton* uibutton_create(float x, float y, float w, float h, UIAnchorPoint uianchor,
                            SDL_Color bg_color, SDL_Color border_color, SDL_Color* bg_color_hovered, SDL_Color* border_color_hovered,
                            void (*on_click)(void* data));

void uibutton_free(UIButton* uibutton);

#endif