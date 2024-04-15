#ifndef UI_MODULE_H
#define UI_MODULE_H

#include <stdlib.h>

#include "button.h"
#include "image.h"
#include "label.h"

typedef enum _UIElementType {
  UIET_Label,
  UIET_Image,
  UIET_Button,
  UIET_Textbox,
} UIElementType;

typedef struct _UIElement {
  UIElementType type;
  void *element;
} UIElement;

typedef struct _UIModule {
  int id;
  int nelements;
  UIElement **elements;
} UIModule;

UIModule *uimodule_create(int id, int nelements);

void uimodule_free(UIModule *module);

void uimodule_set_element(UIModule *module, int index, UIElementType type,
                          void *element);

void update_uimodule(SDL_Renderer *r, UIModule *module);

#endif
