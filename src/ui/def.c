#include "../../include/ui/def.h"
#include <SDL2/SDL_ttf.h>

#define NMODULES_MENU 4
#define NMODULES_INGAME 2

UIModule **get_ui_menu(SDL_Renderer *r, int *nuimodules) {
  *nuimodules = NMODULES_MENU;
  UIModule **modules = malloc(*nuimodules * sizeof(UIModule *));

  // INFO: Substates
  // - 0: menu
  // - 1: settings
  // - 2: connection screen

  // INFO: Module 0: Background
  // available for all substates

  // Background

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

  modules[1] = uimodule_create(1, 5);

  float mm_settings_x = 0.3;
  float mm_settings_y = 0.8;
  float mm_settings_w = 0.4;
  float mm_settings_h = 0.09;

  // Logo

  as = malloc(1 * sizeof(int));
  as[0] = 0;

  UIAnchorPoint anchor = UIAP_CENTER;

  UIImage *logo = uiimage_create(r, 0.2, 0.15, 0.6, 0.4, UIAP_TOP_LEFT, as, 1,
                                 UIIF_FIT, "assets/logo.png", &anchor);
  uimodule_set_element(modules[1], 0, UIET_Image, logo);

  // Settings button

  as = malloc(1 * sizeof(int));
  as[0] = 0;

  SDL_Color mm_settings_button_bg = {
      .r = 0x10, .g = 0x10, .b = 0x10, .a = 0xFF};
  SDL_Color mm_settings_button_bo = {
      .r = 0x30, .g = 0x30, .b = 0x30, .a = 0xFF};
  UIButton *mm_settings_button = uibutton_create(
      mm_settings_x, mm_settings_y, mm_settings_w, mm_settings_h, UIAP_TOP_LEFT,
      as, 1, mm_settings_button_bg, mm_settings_button_bo,
      mm_settings_button_bg, mm_settings_button_bo, UIEC_SetSubstate1);
  uimodule_set_element(modules[1], 1, UIET_Button, mm_settings_button);

  // Settings label

  as = malloc(1 * sizeof(int));
  as[0] = 0;

  TTF_Font *font = TTF_OpenFont("fonts/jersey25.ttf", 25);

  UILabel *mm_settings_label = uilabel_create(
      mm_settings_x, mm_settings_y, mm_settings_w, mm_settings_h, UIAP_TOP_LEFT,
      as, 1, UIAP_CENTER, "settings", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[1], 2, UIET_Label, mm_settings_label);

  // Connect button

  as = malloc(1 * sizeof(int));
  as[0] = 0;

  float mm_connect_y = 0.7;

  UIButton *mm_connect_button = uibutton_create(
      mm_settings_x, mm_connect_y, mm_settings_w, mm_settings_h, UIAP_TOP_LEFT,
      as, 1, mm_settings_button_bg, mm_settings_button_bo,
      mm_settings_button_bg, mm_settings_button_bo, UIEC_SetSubstate2);
  uimodule_set_element(modules[1], 3, UIET_Button, mm_connect_button);

  // Connect label

  as = malloc(1 * sizeof(int));
  as[0] = 0;

  font = TTF_OpenFont("fonts/jersey25.ttf", 25);

  UILabel *mm_connect_label = uilabel_create(
      mm_settings_x, mm_connect_y, mm_settings_w, mm_settings_h, UIAP_TOP_LEFT,
      as, 1, UIAP_CENTER, "connect", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[1], 4, UIET_Label, mm_connect_label);

  // INFO: Module 2: Settings
  // substate 1

  modules[2] = uimodule_create(2, 3);

  // title

  as = malloc(1 * sizeof(int));
  as[0] = 1;

  font = TTF_OpenFont("fonts/jersey25.ttf", 100);

  UILabel *se_title =
      uilabel_create(0, 0, 1, 0.2, UIAP_TOP_LEFT, as, 1, UIAP_CENTER,
                     "settings", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[2], 0, UIET_Label, se_title);

  // back button

  as = malloc(1 * sizeof(int));
  as[0] = 1;

  float gl_back_x = 0.4;
  float gl_back_y = 0.9;
  float gl_back_w = 0.2;
  float gl_back_h = 0.08;

  UIButton *co_back_button = uibutton_create(
      gl_back_x, gl_back_y, gl_back_w, gl_back_h, UIAP_TOP_LEFT, as, 1,
      mm_settings_button_bg, mm_settings_button_bo, mm_settings_button_bg,
      mm_settings_button_bo, UIEC_SetSubstate0);
  uimodule_set_element(modules[2], 1, UIET_Button, co_back_button);

  // back label

  as = malloc(1 * sizeof(int));
  as[0] = 1;

  font = TTF_OpenFont("fonts/jersey25.ttf", 25);

  UILabel *co_back_label =
      uilabel_create(gl_back_x, gl_back_y, gl_back_w, gl_back_h, UIAP_TOP_LEFT,
                     as, 1, UIAP_CENTER, "back", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[2], 2, UIET_Label, co_back_label);

  // INFO: Module 3: Connecting
  // substate 2

  modules[3] = uimodule_create(3, 3);

  // title

  as = malloc(1 * sizeof(int));
  as[0] = 2;

  font = TTF_OpenFont("fonts/jersey25.ttf", 100);

  UILabel *co_title =
      uilabel_create(0, 0, 1, 0.2, UIAP_TOP_LEFT, as, 1, UIAP_CENTER,
                     "connect...", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[3], 0, UIET_Label, co_title);

  // back button

  as = malloc(1 * sizeof(int));
  as[0] = 2;

  UIButton *se_back_button = uibutton_create(
      gl_back_x, gl_back_y, gl_back_w, gl_back_h, UIAP_TOP_LEFT, as, 1,
      mm_settings_button_bg, mm_settings_button_bo, mm_settings_button_bg,
      mm_settings_button_bo, UIEC_SetSubstate0);
  uimodule_set_element(modules[3], 1, UIET_Button, se_back_button);

  // back label

  as = malloc(1 * sizeof(int));
  as[0] = 2;

  font = TTF_OpenFont("fonts/jersey25.ttf", 25);

  UILabel *se_back_label =
      uilabel_create(gl_back_x, gl_back_y, gl_back_w, gl_back_h, UIAP_TOP_LEFT,
                     as, 1, UIAP_CENTER, "back", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[3], 2, UIET_Label, se_back_label);

  return modules;
}

UIModule **get_ui_ingame(SDL_Renderer *r, int *nuimodules) {
  *nuimodules = NMODULES_INGAME;
  UIModule **modules = malloc(*nuimodules * sizeof(UIModule *));

  // INFO: Module 0: crosshair

  modules[0] = uimodule_create(0, 1);

  // crosshair

  int *as = malloc(1 * sizeof(int));
  as[0] = 0;

  UIAnchorPoint anchor = UIAP_CENTER;

  UIImage *crosshair =
      uiimage_create(r, 0.5, 0.5, 0.035, 0.035, UIAP_CENTER, as, 1, UIIF_FIT,
                     "assets/xhair.png", &anchor);

  uimodule_set_element(modules[0], 0, UIET_Image, crosshair);

  // INFO: Module 1 : HUD

  modules[1] = uimodule_create(1, 4);

  // health

  as = malloc(1 * sizeof(int));
  as[0] = 0;

  TTF_Font *font = TTF_OpenFont("fonts/jersey25.ttf", 60);

  UILabel *hud_health =
      uilabel_create(0.05, 0.95, 1, 0.1, UIAP_BOTTOM_LEFT, as, 1,
                     UIAP_BOTTOM_LEFT, "100", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[1], 0, UIET_Label, hud_health);

  // ammo (max)

  as = malloc(1 * sizeof(int));
  as[0] = 0;

  font = TTF_OpenFont("fonts/jersey25.ttf", 30);

  UILabel *hud_ammo_max =
      uilabel_create(0.95, 0.95, 0.1, 0.1, UIAP_BOTTOM_RIGHT, as, 1,
                     UIAP_BOTTOM_LEFT, "/16", 0xAA, 0xAA, 0xAA, 0xFF, font);

  uimodule_set_element(modules[1], 1, UIET_Label, hud_ammo_max);

  // ammo (total)

  as = malloc(1 * sizeof(int));
  as[0] = 0;

  font = TTF_OpenFont("fonts/jersey25.ttf", 40);

  UILabel *hud_ammo_total =
      uilabel_create(0.95, 0.95, 0.1, 0.1, UIAP_BOTTOM_RIGHT, as, 1,
                     UIAP_TOP_RIGHT, "69", 0xCC, 0xCC, 0xCC, 0xFF, font);

  uimodule_set_element(modules[1], 2, UIET_Label, hud_ammo_total);

  // ammo

  as = malloc(1 * sizeof(int));
  as[0] = 0;

  font = TTF_OpenFont("fonts/jersey25.ttf", 60);

  UILabel *hud_ammo =
      uilabel_create(0.85, 0.95, 0.1, 0.1, UIAP_BOTTOM_RIGHT, as, 1,
                     UIAP_BOTTOM_RIGHT, "14", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[1], 3, UIET_Label, hud_ammo);

  return modules;
}

void free_ui(UIModule **yay, int nuimodules) {
  for (int i = 0; i < nuimodules; i++) {
    uimodule_free(yay[i]);
  }
  free(yay);
}
