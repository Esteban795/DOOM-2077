#include "../../include/ui/textbox.h"

#define MOUSE_X_INDEX (NUM_MOUSE_BUTTONS + 2)
#define MOUSE_Y_INDEX (NUM_MOUSE_BUTTONS + 3)
#define MOUSE_LEFT 0

UITextBox *uitextbox_create(float x, float y, float w, float h,
                            UIAnchorPoint anchor, int *as, int nas,
                            TTF_Font *font, UIAnchorPoint text_anchor,
                            int buffer_size, SDL_Color bg, SDL_Color border,
                            SDL_Color text_color, SDL_Scancode scancode) {
  UITextBox *tb = malloc(sizeof(UITextBox));

  tb->common.x = x;
  tb->common.y = y;
  tb->common.width = w;
  tb->common.height = h;
  tb->common.anchor = anchor;
  tb->common.active = true;
  tb->common.active_substates = as;
  tb->common.n_active_substates = nas;

  tb->buffer_size = buffer_size;

  tb->bg_color = bg;
  tb->border_color = border;
  tb->text_color = text_color;

  tb->text_index = 0;
  tb->text = malloc((tb->buffer_size + 1) * sizeof(char));
  tb->text[0] = '\0';

  tb->font = font;
  tb->anchor = text_anchor;

  tb->focused = false;

  tb->scancode = scancode;
  tb->scancode_helper = false;

  return tb;
}

void uitextbox_free(UITextBox *tb) {
  if (tb->common.active_substates) {
    free(tb->common.active_substates);
  }
  free(tb->text);
  free(tb);
}

void uitextbox_update(SDL_Renderer *r, int substate, UITextBox *tb) {
  if (!tb->common.active) {
    return;
  }

  bool found_in_substates = false;
  for (int i = 0; i < tb->common.n_active_substates; i++) {
    found_in_substates |= substate == tb->common.active_substates[i];
  }
  if (!found_in_substates) {
    return;
  }

  SDL_Rect destrect;
  get_absolute_position(&(tb->common), &destrect);

  SDL_Color bg = tb->bg_color;
  SDL_Color border = tb->border_color;

  if (tb->focused) {
    bg = border;
  }

  SDL_SetRenderDrawColor(r, bg.r, bg.g, bg.b, bg.a);
  SDL_RenderFillRect(r, &destrect);

  SDL_SetRenderDrawColor(r, border.r, border.g, border.b, border.a);
  SDL_RenderDrawRect(r, &destrect);

  if (mouse[MOUSE_LEFT]) {
    tb->focused = (mouse[MOUSE_X_INDEX] >= destrect.x &&
                   mouse[MOUSE_X_INDEX] <= destrect.x + destrect.y &&
                   mouse[MOUSE_Y_INDEX] >= destrect.y &&
                   mouse[MOUSE_Y_INDEX] <= destrect.y + destrect.h);
  }

  if (keys[tb->scancode]) {
    if (!tb->scancode_helper){
      tb->focused = !tb->focused;
      textinput[0] = '\0';
    }

    tb->scancode_helper = true;
  } else {
    tb->scancode_helper = false;
  }

  static bool bspace_buffer = false;
  if (!bspace_buffer && keys[SDL_SCANCODE_BACKSPACE]) {
    uitextbox_char_remove(tb);
  }
  bspace_buffer = keys[SDL_SCANCODE_BACKSPACE];

  if (tb->focused && textinput[0] != '\0') {
    uitextbox_string_add(tb);
  }

  SDL_Surface *text =
      TTF_RenderText_Blended(tb->font, tb->text, tb->text_color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(r, text);

  int tw, th;
  TTF_SizeText(tb->font, tb->text, &tw, &th);

  switch (tb->anchor) {
  case UIAP_CENTER:
    destrect.x += destrect.w / 2 - tw / 2;
    destrect.y += destrect.h / 2 - th / 2;
    break;
  case UIAP_BOTTOM_RIGHT:
    destrect.x += destrect.w - tw;
    destrect.y += destrect.h - th;
    break;
  case UIAP_BOTTOM_LEFT:
    destrect.y += destrect.h - th;
    break;
  case UIAP_TOP_RIGHT:
    destrect.x += destrect.w - tw;
    break;
  default:
    break;
  }

  destrect.w = tw;
  destrect.h = th;

  SDL_RenderCopy(r, texture, NULL, &destrect);
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(text);
}

void uitextbox_char_add(UITextBox *tb, char c) {
  if (tb->text_index < tb->buffer_size) {
    tb->text[tb->text_index] = c;
    tb->text_index++;
    tb->text[tb->text_index] = '\0';
  }
}

void uitextbox_string_add(UITextBox *tb) {
  for (int i = 0; textinput[i] != '\0'; i++) {
    uitextbox_char_add(tb, textinput[i]);
    textinput[i] = '\0';
  }
}

void uitextbox_char_remove(UITextBox *tb) {
  if (tb->text_index > 0) {
    tb->text_index--;
    tb->text[tb->text_index] = '\0';
  }
}
