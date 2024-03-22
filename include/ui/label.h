#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "common.h"

#include <stdlib.h>

typedef struct _UILabel {
    UICommon common;
    UIAnchorPoint text_anchor;
    char* string;
    SDL_Color color;
} UILabel;

UILabel* uilabel_create(float x, float y, float w, float h,
                        UIAnchorPoint m_anchor, UIAnchorPoint text_anchor, char* string,
                        int r, int g, int b, int a);

void uilabel_free(UILabel* o);

void uilabel_render(UILabel* o);

#endif