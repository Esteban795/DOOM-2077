#include "../../include/ui/def.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_ttf.h>

#define NMODULES_MENU 1
#define NMODULES_INGAME 0

UIModule **get_ui_menu(SDL_Renderer *r, int *nuimodules) {
  *nuimodules = NMODULES_MENU;
  UIModule **modules = malloc(*nuimodules * sizeof(UIModule *));

  modules[0] = uimodule_create(0, 1);

  int *as = malloc(3 * sizeof(int));
  as[0] = 0;
  as[1] = 1;
  as[2] = 2;
  UIImage *image =
      uiimage_create(r, 0, 0, 1, 1, UIAP_TOP_LEFT, as, 3, UIIF_STRETCH,
                     "assets/giygas_tiled.png", NULL);

  uimodule_set_element(modules[0], 0, UIET_Image, image);

  return modules;
}

UIModule **get_ui_ingame(SDL_Renderer *r, int *nuimodules) {
  *nuimodules = NMODULES_INGAME;
  UIModule **modules = malloc(*nuimodules * sizeof(UIModule *));

  return modules;
}

void free_ui(UIModule **yay, int nuimodules) {
  for (int i = 0; i < nuimodules; i++) {
    uimodule_free(yay[i]);
  }
  free(yay);
}
