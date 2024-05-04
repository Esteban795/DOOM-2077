#include "../../include/ui/label.h"

UILabel *uilabel_create(float x, float y, float w, float h,
                        UIAnchorPoint m_anchor, int *as, int nas,
                        UIAnchorPoint text_anchor, char *string, int r, int g,
                        int b, int a, TTF_Font *font) {
  UILabel *uilabel = malloc(sizeof(UILabel));

  uilabel->common.x = x;
  uilabel->common.y = y;
  uilabel->common.width = w;
  uilabel->common.height = h;
  uilabel->common.anchor = m_anchor;
  uilabel->common.active = true;
  uilabel->common.active_substates = as;
  uilabel->common.n_active_substates = nas;

  uilabel->text_anchor = text_anchor;
  uilabel->string = string;

  uilabel->color.r = r;
  uilabel->color.g = g;
  uilabel->color.b = b;
  uilabel->color.a = a;

  uilabel->font = font;

  return uilabel;
}

void uilabel_free(UILabel *uilabel) {
  if (uilabel->common.active_substates) {
    free(uilabel->common.active_substates);
  }
  free(uilabel);
}

void uilabel_update(SDL_Renderer *r, int substate, UILabel *uilabel) {
  if (!uilabel->common.active) {
    return;
  }

  bool found_in_substates = false;
  for (int i = 0; i < uilabel->common.n_active_substates; i++) {
    found_in_substates |= substate == uilabel->common.active_substates[i];
  }
  if (!found_in_substates) {
    return;
  }

  SDL_Surface *text =
      TTF_RenderText_Blended(uilabel->font, uilabel->string, uilabel->color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(r, text);

  int tw, th;
  TTF_SizeText(uilabel->font, uilabel->string, &tw, &th);

  SDL_Rect destrect;
  get_absolute_position(&(uilabel->common), &destrect);

  switch (uilabel->text_anchor) {
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
  return;
}
