#include "../../include/ui/def.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_ttf.h>

#define NMODULES_MENU 1
#define NMODULES_INGAME 0

UIModule **get_ui_menu(SDL_Renderer *r, int *nuimodules) {
  *nuimodules = NMODULES_MENU;
  UIModule **modules = malloc(*nuimodules * sizeof(UIModule *));

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
