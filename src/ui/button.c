#include "../../include/ui/button.h"

UIButton* uibutton_create(float x, float y, float w, float h, UIAnchorPoint uianchor,
                            SDL_Color bg_color, SDL_Color border_color, SDL_Color bg_color_pressed, SDL_Color border_color_pressed,
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
    b->bg_color_pressed = bg_color_pressed;
    b->border_color = border_color;
    b->border_color_pressed = border_color_pressed;

    b->on_click = on_click;

    return b;
}

void uibutton_free(UIButton* uibutton){
    free(uibutton);
}

void uibutton_detect_click(UIButton* uibutton, SDL_Rect* rect, bool mouse_left_state){
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    if (mouse_x > rect->x + rect->w || mouse_x < rect->x || mouse_y > rect->y + rect->h ||
        mouse_y < rect->y) {
        uibutton->pressed = false;
        return;
    } else if ( mouse_left_state ) {
        if (!uibutton->pressed){
            uibutton->on_click();
        }
        uibutton->pressed = true;
    }
    
}

void uibutton_update(SDL_Renderer* r, UIButton* uibutton, const uint8_t* keys){
    SDL_Rect destrect;
    get_absolute_position(&(uibutton->common), &destrect);

    uibutton_detect_click(uibutton, &destrect, keys[SDL_SCANCODE_P] > 0);

    SDL_Color bg = uibutton->bg_color;
    SDL_Color border = uibutton->border_color;
    if (uibutton->pressed) {
        bg = uibutton->bg_color_pressed;
        border = uibutton->border_color_pressed;
    }

    SDL_SetRenderDrawColor(r, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderFillRect(r, &destrect);

    SDL_SetRenderDrawColor(r, border.r, border.g, border.b, border.a);
    SDL_RenderDrawRect(r, &destrect);
}