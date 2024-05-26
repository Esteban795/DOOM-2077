#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "common.h"

#include <stdlib.h>

typedef struct _UILabel {
  UICommon common;
  UIAnchorPoint text_anchor;
  char *string;
  SDL_Color color;
  TTF_Font *font;
} UILabel;

UILabel *uilabel_create(float x, float y, float w, float h,
                        UIAnchorPoint m_anchor, int *as, int nas,
                        UIAnchorPoint text_anchor, char *string, int r, int g,
                        int b, int a, TTF_Font *font);

/// @brief Set the content of a UILabel
/// @param uilabel the UILabel to set the content of
/// @param string a string to set the UILabel to
/// @note This function will reallocate memory for the string, so it is safe to
/// pass a string that is not dynamically allocated
void uilabel_set_content(UILabel *uilabel, char *string);

void uilabel_free(UILabel *o);

void uilabel_update(SDL_Renderer *r, int substate, UILabel *o);

#endif
