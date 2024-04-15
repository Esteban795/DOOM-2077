#include "../../include/ui/module.h"

UIModule *uimodule_create(int id, int nelements) {
  UIModule *m = malloc(sizeof(UIModule));

  m->id = id;
  m->nelements = nelements;
  m->elements = malloc(nelements * sizeof(UIElement *));
  for (int i = 0; i < nelements; i++) {
    m->elements[i] = 0;
  }

  return m;
}

void uimodule_free(UIModule *module) {
  for (int i = 0; i < module->nelements; i++) {
    if (module->elements[i] != 0) {
      switch (module->elements[i]->type) {
      case UIET_Label:
        uilabel_free(module->elements[i]->element);
        break;
      case UIET_Button:
        uibutton_free(module->elements[i]->element);
        break;
      case UIET_Image:
        uiimage_free(module->elements[i]->element);
        break;
      default:
        break;
      }
      free(module->elements[i]);
    }
  }
  free(module->elements);
  free(module);
}

void uimodule_set_element(UIModule *module, int index, UIElementType type,
                          void *element) {
  if (index < module->nelements) {
    module->elements[index] = malloc(sizeof(UIElement));
    module->elements[index]->type = type;
    module->elements[index]->element = element;
  }
}

void update_uimodule(SDL_Renderer *r, UIModule *module) {
  for (int i = 0; i < module->nelements; i++) {
    if (module->elements[i] != 0) {
      switch (module->elements[i]->type) {
      case UIET_Label:
        uilabel_update(r, module->elements[i]->element);
        break;
      case UIET_Button:
        // TODO: fix this to work with the new event handler

        // uibutton_update(r, module->elements[i]->element, keys);
        break;
      case UIET_Image:
        uiimage_update(r, module->elements[i]->element);
        break;
      default:
        break;
      }
    }
  }
}
