#include "../../include/ui/button.h"

UIButton* uibutton_create(float x, float y, float w, float h, UIAnchorPoint uianchor,
                            SDL_Color bg_color, SDL_Color border_color, SDL_Color* bg_color_hovered, SDL_Color* border_color_hovered,
                            void (*on_click)(void)){
    UIButton* b = malloc(sizeof(UIButton));

    b->common.x = x;
    b->common.y = y;
    b->common.width = w;
    b->common.height = h;
    b->common.anchor = uianchor;

    b->pressed = false;
    b->hovered = false;

    b->bg_color = bg_color;
    b->bg_color_hovered = bg_color_hovered;
    b->border_color = border_color;
    b->border_color_hovered = border_color_hovered;

    b->on_click = on_click;

    return b;
}

void uibutton_free(UIButton* uibutton){
    free(uibutton);
}