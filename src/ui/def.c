#include "../../include/ui/def.h"

UIModule **get_ui_ingame(int *nuimodules) {
  *nuimodules = 1;
  UIModule **modules = malloc(*nuimodules * sizeof(UIModule *));

  modules[0] = uimodule_create(0, 1);

  UIImage *image = uiimage_create(0, 0, 0.5, 0.5, UIAP_TOP_LEFT,
                                  "../../assets/testimage.jpg");

  uimodule_set_element(modules[0], 0, UIET_Image, image);

  return modules;
}

void free_ui_ingame(UIModule **yay) {
  for (int i = 0; i < 0; i++) {
    uimodule_free(yay[i]);
  }
  free(yay);
}
