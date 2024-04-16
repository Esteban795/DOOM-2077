#include "../../include/ui/common.h"

void get_absolute_position(UICommon *common, SDL_Rect *rect) {
  rect->w = (int)round(WIDTH * common->width);
  rect->h = (int)round(HEIGHT * common->height);

  switch (common->anchor) {
  case UIAP_TOP_LEFT:
    rect->x = (int)round(WIDTH * common->x);
    rect->y = (int)round(HEIGHT * common->y);
    break;
  case UIAP_TOP_RIGHT:
    rect->x = (int)round(WIDTH * common->x - rect->w);
    rect->y = (int)round(HEIGHT * common->y);
    break;
  case UIAP_BOTTOM_LEFT:
    rect->x = (int)round(WIDTH * common->x);
    rect->y = (int)round(HEIGHT * common->y - rect->h);
    break;
  case UIAP_BOTTOM_RIGHT:
    rect->x = (int)round(WIDTH * common->x - rect->w);
    rect->y = (int)round(HEIGHT * common->y - rect->h);
    break;
  case UIAP_CENTER:
    rect->x = (int)round(WIDTH * common->x - rect->w / 2);
    rect->y = (int)round(HEIGHT * common->y - rect->h / 2);
    break;
  }
}
