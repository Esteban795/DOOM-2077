#ifndef UI_COMMON_H
#define UI_COMMON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "../events.h"
#include "../settings.h"

// represents where the UI element sits compared to its positioning values
// (x,y,w,h)
typedef enum _UIAnchorPoint {
  UIAP_TOP_LEFT,
  UIAP_TOP_RIGHT,
  UIAP_BOTTOM_LEFT,
  UIAP_BOTTOM_RIGHT,
  UIAP_CENTER
} UIAnchorPoint;

// this runs "relative to viewport" positioning
// x=1 represents the right-most pixel of the screen, x=0 the left-most pixel
typedef struct _UICommon {
  bool active;

  float x;      // in [0,1]
  float y;      // in [0,1]
  float width;  // in [0,1]
  float height; // in [0,1]

  UIAnchorPoint anchor;

  int *active_substates;
  int n_active_substates;
} UICommon;

void get_absolute_position(UICommon *common, SDL_Rect *rect);

#endif
