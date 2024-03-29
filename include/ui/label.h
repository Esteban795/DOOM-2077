#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "common.h"

#include <stdlib.h>

typedef struct _UILabel {
    UICommon common;
    UIAnchorPoint text_anchor;
    char* string;
    SDL_Color color;
    TTF_Font* font;
} UILabel;

UILabel* uilabel_create(float x, float y, float w, float h,
                        UIAnchorPoint m_anchor, UIAnchorPoint text_anchor, char* string,
                        int r, int g, int b, int a, TTF_Font* font);

void uilabel_free(UILabel* o);

void uilabel_update(SDL_Renderer* r, UILabel* o);

#endif