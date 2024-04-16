#include "../../include/ui/def.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_ttf.h>

#define NMODULES 0

UIModule **get_ui_ingame(SDL_Renderer *r, int *nuimodules) {
  *nuimodules = NMODULES;
  UIModule **modules = malloc(*nuimodules * sizeof(UIModule *));

  return modules;
}

void free_ui_ingame(UIModule **yay) {
  for (int i = 0; i < NMODULES; i++) {
    uimodule_free(yay[i]);
  }
  free(yay);
}
