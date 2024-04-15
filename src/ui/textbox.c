#include "../../include/ui/textbox.h"

UITextBox *uitextbox_create(float x, float y, float w, float h,
                            UIAnchorPoint anchor, TTF_Font *font,
                            int buffer_size, SDL_Color bg, SDL_Color border,
                            char *placeholder) {
  UITextBox *tb = malloc(sizeof(UITextBox));

  tb->common.x = x;
  tb->common.y = y;
  tb->common.width = w;
  tb->common.height = h;
  tb->common.anchor = anchor;
  tb->common.active = true;

  // INFO: why +1 ? to account for null byte.
  // if you want to limit the textbox to n characters, set the buffer size to
  // exactly that. the code will take care of it all ^ ^
  tb->buffer_size = buffer_size + 1;

  tb->bg_color = bg;
  tb->border_color = border;

  tb->placeholder = placeholder;
  tb->text_index = 0;
  tb->text = malloc(tb->buffer_size * sizeof(char));
  tb->text[0] = '\0';

  tb->font = font;

  tb->focused = false;

  return tb;
}

void uitextbox_free(UITextBox *tb) {
  free(tb->text);
  free(tb);
}

void uitextbox_update(SDL_Renderer *r, UITextBox *tb) {
  if (!tb->common.active) {
    return;
  }

  SDL_Rect destrect;
  get_absolute_position(&(tb->common), &destrect);

  SDL_Color bg = tb->bg_color;
  SDL_Color border = tb->border_color;

  SDL_SetRenderDrawColor(r, bg.r, bg.g, bg.b, bg.a);
  SDL_RenderFillRect(r, &destrect);

  SDL_SetRenderDrawColor(r, border.r, border.g, border.b, border.a);
  SDL_RenderDrawRect(r, &destrect);

  if (mouse[0])
}
