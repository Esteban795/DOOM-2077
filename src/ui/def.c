#include "../../include/ui/def.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_ttf.h>

#define NMODULES_MENU 2
#define NMODULES_INGAME 0

UIModule **get_ui_menu(SDL_Renderer *r, int *nuimodules) {
  *nuimodules = NMODULES_MENU;
  UIModule **modules = malloc(*nuimodules * sizeof(UIModule *));

  // INFO: Substates
  // - 0: menu
  // - 1: settings
  // - 2: connection screen

  // INFO: Module 0: Background
  // available for all substates

  modules[0] = uimodule_create(0, 1);

  int *as = malloc(3 * sizeof(int));
  as[0] = 0;
  as[1] = 1;
  as[2] = 2;
  UIImage *image =
      uiimage_create(r, 0, 0, 1, 1, UIAP_TOP_LEFT, as, 3, UIIF_STRETCH,
                     "assets/giygas_tiled.png", NULL);

  uimodule_set_element(modules[0], 0, UIET_Image, image);

  // INFO: Module 1: Main menu
  // available for substate 0: menu

  modules[1] = uimodule_create(1, 3);

  as = malloc(1 * sizeof(int));
  as[0] = 0;

  UIImage *logo = uiimage_create(r, 0.2, 0.15, 0.6, 0.4, UIAP_TOP_LEFT, as, 1,
                                 UIIF_FIT, "assets/testimage.jpg", NULL);
  uimodule_set_element(modules[1], 0, UIET_Image, logo);

  as = malloc(1 * sizeof(int));
  as[0] = 0;

  SDL_Color settings_button_bg = {.r = 0x10, .g = 0x10, .b = 0x10, .a = 0xFF};
  SDL_Color settings_button_bo = {.r = 0x30, .g = 0x30, .b = 0x30, .a = 0xFF};
  UIButton *settings_button = uibutton_create(
      0.3, 0.7, 0.4, 0.1, UIAP_TOP_LEFT, as, 1, settings_button_bg,
      settings_button_bo, settings_button_bg, settings_button_bo,
      UIEC_SetSubstate1);
  uimodule_set_element(modules[1], 1, UIET_Button, settings_button);

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
