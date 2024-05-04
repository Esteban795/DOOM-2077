#include "../../include/ui/image.h"
#include <SDL2/SDL_render.h>

UIImage *uiimage_create(SDL_Renderer *r, float x, float y, float w, float h,
                        UIAnchorPoint uianchor, int *as, int nas,
                        UIImage_Fit fit, char *filename,
                        UIAnchorPoint *image_anchor) {
  UIImage *uiimage = malloc(sizeof(UIImage));

  uiimage->common.x = x;
  uiimage->common.y = y;
  uiimage->common.width = w;
  uiimage->common.height = h;
  uiimage->common.anchor = uianchor;
  uiimage->common.active = true;
  uiimage->common.active_substates = as;
  uiimage->common.n_active_substates = nas;

  uiimage->surface = IMG_Load(filename);
  uiimage->texture = SDL_CreateTextureFromSurface(r, uiimage->surface);

  uiimage->fit = fit;

  if (fit == UIIF_FIT) {
    if (image_anchor) {
      uiimage->anchor = *(image_anchor);
    } else {
      uiimage->fit = UIIF_STRETCH;
    }
  }

  return uiimage;
}

void uiimage_free(UIImage *uiimage) {
  SDL_DestroyTexture(uiimage->texture);
  SDL_FreeSurface(uiimage->surface);
  if (uiimage->common.active_substates) {
    free(uiimage->common.active_substates);
  }
  free(uiimage);
}

void uiimage_update(SDL_Renderer *r, int substate, UIImage *uiimage) {
  if (!uiimage->common.active) {
    return;
  }

  bool found_in_substates = false;
  for (int i = 0; i < uiimage->common.n_active_substates; i++) {
    found_in_substates |= substate == uiimage->common.active_substates[i];
  }
  if (!found_in_substates) {
    return;
  }

  SDL_Rect destrect;
  get_absolute_position(&(uiimage->common), &destrect);

  if (uiimage->fit == UIIF_STRETCH) {
    SDL_RenderCopy(r, uiimage->texture, NULL, &destrect);
  } else {
    int w = uiimage->surface->w;
    int h = uiimage->surface->h;

    float bbox_ratio = (float)destrect.w / (float)destrect.h;
    float img_ratio = (float)w / (float)h;

    SDL_Rect nrect;

    if (bbox_ratio > img_ratio) {
      nrect.h = destrect.h;
      nrect.w = destrect.h * img_ratio;
      nrect.y = destrect.y;
      switch (uiimage->anchor) {
      case UIAP_CENTER:
        nrect.x = destrect.x + (destrect.w - nrect.w) / 2;
        break;
      case UIAP_TOP_LEFT:
      case UIAP_BOTTOM_LEFT:
        nrect.x = destrect.x;
        break;
      case UIAP_TOP_RIGHT:
      case UIAP_BOTTOM_RIGHT:
        nrect.x = destrect.x + destrect.w - nrect.w;
        break;
      }
    } else {
      nrect.w = destrect.w;
      nrect.h = destrect.w / img_ratio;
      nrect.x = destrect.x;
      switch (uiimage->anchor) {
      case UIAP_CENTER:
        nrect.y = destrect.y + (destrect.h - nrect.h) / 2;
        break;
      case UIAP_TOP_LEFT:
      case UIAP_TOP_RIGHT:
        nrect.y = destrect.y;
        break;
      case UIAP_BOTTOM_LEFT:
      case UIAP_BOTTOM_RIGHT:
        nrect.y = destrect.y + destrect.h - nrect.h;
      }
    }

    SDL_RenderCopy(r, uiimage->texture, NULL, &nrect);
  }
}
