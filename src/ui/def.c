#include "../../include/ui/def.h"

UIModule **get_ui_ingame(SDL_Renderer *r, int *nuimodules) {
  *nuimodules = 1;
  UIModule **modules = malloc(*nuimodules * sizeof(UIModule *));

  modules[0] = uimodule_create(0, 1);

  UIAnchorPoint a = UIAP_CENTER;

  UIImage *image = uiimage_create(r, 0, 0, 0.2, 0.5, UIAP_TOP_LEFT, UIIF_FIT,
                                  "assets/testimage.jpg", &a);

  uimodule_set_element(modules[0], 0, UIET_Image, image);

  return modules;
}

void free_ui_ingame(UIModule **yay) {
  for (int i = 0; i < 1; i++) {
    uimodule_free(yay[i]);
  }
  free(yay);
}
