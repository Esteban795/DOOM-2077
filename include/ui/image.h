#ifndef UI_IMAGE_H
#define UI_IMAGE_H

#include "common.h"
#include <SDL2/SDL_render.h>
#include <stdlib.h>

typedef enum _UIImage_Fit { UIIF_STRETCH, UIIF_FIT } UIImage_Fit;

typedef struct _UIImage {
  UICommon common;
  UIImage_Fit fit;
  UIAnchorPoint anchor;
  SDL_Surface *surface;
  SDL_Texture *texture;
} UIImage;

UIImage *uiimage_create(SDL_Renderer *r, float x, float y, float w, float h,
                        UIAnchorPoint uianchor, int *as, int nas,
                        UIImage_Fit fit, char *filename,
                        UIAnchorPoint *image_anchor);

void uiimage_free(UIImage *uiimage);

void uiimage_update(SDL_Renderer *r, int substate, UIImage *uiimage);

#endif
