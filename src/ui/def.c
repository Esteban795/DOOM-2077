#include "../../include/ui/def.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_ttf.h>

#define NMODULES 1

UIModule **get_ui_ingame(SDL_Renderer *r, int *nuimodules) {
  *nuimodules = NMODULES;
  UIModule **modules = malloc(*nuimodules * sizeof(UIModule *));

  modules[0] = uimodule_create(0, 1);

  SDL_Color bg = {.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF};
  SDL_Color border = {.r = 0x99, .g = 0x99, .b = 0x99, .a = 0xFF};
  SDL_Color text = {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0xFF};

  TTF_Font *font = TTF_OpenFont("fonts/lemon_milk/LEMONMILK-Regular.otf", 50);
  printf("%s\n", SDL_GetError());

  UITextBox *tb =
      uitextbox_create(0.25, 0.25, 0.5, 0.5, UIAP_TOP_LEFT, font, UIAP_TOP_LEFT,
                       10, bg, border, text, "ta_mere_EN_Slip.");

  uimodule_set_element(modules[0], 0, UIET_Textbox, tb);

  return modules;
}

void free_ui_ingame(UIModule **yay) {
  for (int i = 0; i < NMODULES; i++) {
    uimodule_free(yay[i]);
  }
  free(yay);
}
