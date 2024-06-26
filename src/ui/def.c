#include "../../include/ui/def.h"
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_ttf.h>

#define NMODULES_MENU 4
#define NMODULES_INGAME 5

void create_setting(UIModule* module, int gridx, int gridy, char* label_text, int label_id, int textbox_id, SDL_Color mm_settings_button_bg, SDL_Color mm_settings_button_bo, SDL_Color white){
  int* as = malloc(1 * sizeof(int));
  as[0] = 1;
  TTF_Font* font = TTF_OpenFont("fonts/jersey25.ttf", 25);
  UILabel * label_ptr =
      uilabel_create(0.2*( gridx +1)-0.01, 0.1*( gridy +2.5)+0.005, 0.09, 0.04, UIAP_TOP_RIGHT,
                     as, 1, UIAP_TOP_RIGHT, label_text, 0xFF, 0xFF, 0xFF, 0xFF, font);
  uimodule_set_element(module, label_id , UIET_Label, label_ptr );
  as = malloc(1 * sizeof(int));
  as[0] = 1;
  font = TTF_OpenFont("fonts/jersey25.ttf", 25);
  UITextBox * textbox_ptr =
      uitextbox_create(0.2*( gridx +1)+0.01, 0.1*( gridy +2.5)+0.005, 0.09, 0.04, UIAP_TOP_LEFT, as, 1, font, UIAP_TOP_LEFT, 6,
      mm_settings_button_bg, mm_settings_button_bo, white, SDL_SCANCODE_F20);
  uimodule_set_element(module, textbox_id , UIET_Textbox, textbox_ptr );
}

UIModule **get_ui_menu(SDL_Renderer *r, int *nuimodules)
{
  SDL_Color white = {.r = 0xff, .g = 0xff, .b = 0xff, .a = 0xff};

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
      uiimage_create(r, 0, 0, 1, 1, UIAP_TOP_LEFT, as, 3, UIIF_STRETCH, "assets/giygas_tiled.png", NULL);

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

  UIImage *logo = uiimage_create(r, 0.2, 0.15, 0.6, 0.4, UIAP_TOP_LEFT, as, 1, UIIF_FIT, "assets/logo.png", &anchor);
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
      mm_settings_button_bg, mm_settings_button_bo, UIEC_SetSubstate1, SDL_SCANCODE_S);
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

  float mm_connect_y = 0.65;

  UIButton *mm_connect_button = uibutton_create(
      mm_settings_x, mm_connect_y, mm_settings_w, mm_settings_h, UIAP_TOP_LEFT,
      as, 1, mm_settings_button_bg, mm_settings_button_bo,
      mm_settings_button_bg, mm_settings_button_bo, UIEC_SetSubstate2, SDL_SCANCODE_C);
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

  modules[2] = uimodule_create(2, 5 + 8 * 2);

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
      gl_back_x + 0.1 + gl_back_w / 2, gl_back_y, gl_back_w, gl_back_h, UIAP_TOP_LEFT, as, 1,
      mm_settings_button_bg, mm_settings_button_bo, mm_settings_button_bg,
      mm_settings_button_bo, UIEC_SetSubstate0, SDL_SCANCODE_F14);
  uimodule_set_element(modules[2], 1, UIET_Button, co_back_button);

  // back label

  as = malloc(1 * sizeof(int));
  as[0] = 1;

  font = TTF_OpenFont("fonts/jersey25.ttf", 25);

  UILabel *co_back_label =
      uilabel_create(gl_back_x + 0.1 + gl_back_w / 2, gl_back_y, gl_back_w, gl_back_h, UIAP_TOP_LEFT,
                     as, 1, UIAP_CENTER, "back", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[2], 2, UIET_Label, co_back_label);

  // back button

  as = malloc(1 * sizeof(int));
  as[0] = 1;

  UIButton *se_save_button = uibutton_create(
      gl_back_x - 0.1 - gl_back_w / 2, gl_back_y, gl_back_w, gl_back_h, UIAP_TOP_LEFT, as, 1,
      mm_settings_button_bg, mm_settings_button_bo, mm_settings_button_bg,
      mm_settings_button_bo, UIEC_SaveSettings, SDL_SCANCODE_S);
  uimodule_set_element(modules[2], 3, UIET_Button, se_save_button);

  // back label

  as = malloc(1 * sizeof(int));
  as[0] = 1;

  font = TTF_OpenFont("fonts/jersey25.ttf", 25);

  UILabel *se_save_label =
      uilabel_create(gl_back_x - 0.1 - gl_back_w / 2, gl_back_y, gl_back_w, gl_back_h, UIAP_TOP_LEFT,
                     as, 1, UIAP_CENTER, "save", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[2], 4, UIET_Label, se_save_label);

  // KEYBINDS

  create_setting(modules[2], 0, 0, "left", 5, 6, mm_settings_button_bg, mm_settings_button_bo, white);
  create_setting(modules[2], 1, 0, "right", 7, 8, mm_settings_button_bg, mm_settings_button_bo, white);
  create_setting(modules[2], 2, 0, "up", 9, 10, mm_settings_button_bg, mm_settings_button_bo, white);
  create_setting(modules[2], 3, 0, "down", 11, 12, mm_settings_button_bg, mm_settings_button_bo, white);
  create_setting(modules[2], 0, 1, "reload", 13, 14, mm_settings_button_bg, mm_settings_button_bo, white);
  create_setting(modules[2], 1, 1, "intrct", 15, 16, mm_settings_button_bg, mm_settings_button_bo, white);

  // SETTINGS
  create_setting(modules[2], 0, 3, "sens", 17, 18, mm_settings_button_bg, mm_settings_button_bo, white);
  create_setting(modules[2], 1, 3, "fov", 19, 20, mm_settings_button_bg, mm_settings_button_bo, white);

  // INFO: Module 3: Connecting
  // substate 2

  modules[3] = uimodule_create(3, 9);

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
      mm_settings_button_bo, UIEC_SetSubstate0, SDL_SCANCODE_F13);
  uimodule_set_element(modules[3], 1, UIET_Button, se_back_button);

  // back label

  as = malloc(1 * sizeof(int));
  as[0] = 2;

  font = TTF_OpenFont("fonts/jersey25.ttf", 25);

  UILabel *se_back_label =
      uilabel_create(gl_back_x, gl_back_y, gl_back_w, gl_back_h, UIAP_TOP_LEFT,
                     as, 1, UIAP_CENTER, "back", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[3], 2, UIET_Label, se_back_label);

  // connect button

  as = malloc(1 * sizeof(int));
  as[0] = 2;

  UIButton *co_co_button = uibutton_create(
      gl_back_x - 0.1, gl_back_y - 0.15, gl_back_w + 0.2, gl_back_h, UIAP_TOP_LEFT, as, 1,
      mm_settings_button_bg, mm_settings_button_bo, mm_settings_button_bg,
      mm_settings_button_bo, UIEC_JoinServer, SDL_SCANCODE_F14);
  uimodule_set_element(modules[3], 3, UIET_Button, co_co_button);

  // back label

  as = malloc(1 * sizeof(int));
  as[0] = 2;

  font = TTF_OpenFont("fonts/jersey25.ttf", 25);

  UILabel *co_co_label =
      uilabel_create(gl_back_x - 0.1, gl_back_y - 0.15, gl_back_w + 0.2, gl_back_h, UIAP_TOP_LEFT,
                     as, 1, UIAP_CENTER, "connect", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[3], 4, UIET_Label, co_co_label);

  as = malloc(1 * sizeof(int));
  as[0] = 2;

  font = TTF_OpenFont("fonts/jersey25.ttf", 30);

  UILabel *co_name_label =
      uilabel_create(0.05, 0.35, 0.2, 0.07, UIAP_TOP_LEFT,
                     as, 1, UIAP_TOP_RIGHT, "nickname", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[3], 5, UIET_Label, co_name_label);

  as = malloc(1 * sizeof(int));
  as[0] = 2;

  font = TTF_OpenFont("fonts/jersey25.ttf", 30);

  UITextBox *co_name_tb = uitextbox_create(0.35, 0.35, 0.5, 0.07, UIAP_TOP_LEFT, as, 1, font, UIAP_TOP_LEFT, 24, mm_settings_button_bg, mm_settings_button_bo, white, SDL_SCANCODE_LSHIFT);

  uimodule_set_element(modules[3], 6, UIET_Textbox, co_name_tb);

  as = malloc(1 * sizeof(int));
  as[0] = 2;

  font = TTF_OpenFont("fonts/jersey25.ttf", 30);

  UILabel *co_ip_label =
      uilabel_create(0.05, 0.5, 0.2, 0.07, UIAP_TOP_LEFT,
                     as, 1, UIAP_TOP_RIGHT, "ip", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[3], 7, UIET_Label, co_ip_label);

  as = malloc(1 * sizeof(int));
  as[0] = 2;

  font = TTF_OpenFont("fonts/jersey25.ttf", 30);

  UITextBox *co_ip_tb = uitextbox_create(0.35, 0.5, 0.5, 0.07, UIAP_TOP_LEFT, as, 1, font, UIAP_TOP_LEFT, 22, mm_settings_button_bg, mm_settings_button_bo, white, SDL_SCANCODE_LSHIFT);
  char *default_ip = "127.0.0.1";
  int len = strlen(default_ip);
  memcpy(co_ip_tb->text, default_ip, len+1);

  uimodule_set_element(modules[3], 8, UIET_Textbox, co_ip_tb);

  return modules;
}

// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠈⣿⣿⣿⣿⣿⣿⠟⠉⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⡿⠋⠀⠀⠙⢻⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⣠⣄⠀⢻⣿⣿⣿⣿⣿⡿⠀⣠⣄⠀⠀⠀⢻⣿⣿⣏⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⣾⣿⣿⣿⣿⠀⠀⠀⠀⠰⣿⣿⠀⢸⣿⣿⣿⣿⣿⡇⠀⣿⣿⡇⠀⠀⢸⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣄⠀⠀⠀⠀⠙⠃⠀⣼⣿⣿⣿⣿⣿⣇⠀⠙⠛⠁⠀⠀⣼⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣷⣤⣄⣀⣠⣤⣾⣿⣿⣿⣿⣽⣿⣿⣦⣄⣀⣀⣤⣾⣿⣿⣿⣿⠃⠀⠀⢀⣀⠀⠀
// ⠰⡶⠶⠶⠶⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠛⠉⠉⠙⠛⠋⠀
// ⠀⠀⢀⣀⣠⣤⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠷⠶⠶⠶⢤⣤⣀⠀
// ⠀⠛⠋⠉⠁⠀⣀⣴⡿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣤⣀⡀⠀⠀⠀⠀⠘⠃
// ⠀⠀⢀⣤⡶⠟⠉⠁⠀⠀⠉⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠟⠉⠀⠀⠀⠉⠙⠳⠶⣄⡀⠀⠀
// ⠀⠀⠙⠁⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠁⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// ⠀⠀⠀⠀⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
// hi, i am Separator the Cat! :3
// my purpose is to be visible when scrolling through this code horror! :)

UIModule **get_ui_ingame(SDL_Renderer *r, int *nuimodules)
{
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

  modules[1] = uimodule_create(1, 3);

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
      uilabel_create(1, 0.95, 0.1, 0.1, UIAP_BOTTOM_RIGHT, as, 1,
                     UIAP_BOTTOM_LEFT, "/16", 0xAA, 0xAA, 0xAA, 0xFF, font);

  uimodule_set_element(modules[1], 1, UIET_Label, hud_ammo_max);

  // ammo

  as = malloc(1 * sizeof(int));
  as[0] = 0;

  font = TTF_OpenFont("fonts/jersey25.ttf", 60);

  UILabel *hud_ammo =
      uilabel_create(0.9, 0.95, 0.1, 0.1, UIAP_BOTTOM_RIGHT, as, 1,
                     UIAP_BOTTOM_RIGHT, "14", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[1], 2, UIET_Label, hud_ammo);

  // INFO: Module 2: killfeed

  modules[2] = uimodule_create(2, 4);

  // label
  as = malloc(1 * sizeof(int));
  as[0] = 0;

  font = TTF_OpenFont("fonts/jersey25.ttf", 30);

  UILabel *hud_killfeed_label_a =
      uilabel_create(0.01, 0, 0.5, 0.5, UIAP_TOP_LEFT, as, 1,
                     UIAP_TOP_LEFT, "\0", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[2], 1, UIET_Label, hud_killfeed_label_a);

  as = malloc(1 * sizeof(int));
  as[0] = 0;

  font = TTF_OpenFont("fonts/jersey25.ttf", 25);

  UILabel *hud_killfeed_label_b =
      uilabel_create(0.02, 0.047, 0.5, 0.5, UIAP_TOP_LEFT, as, 1,
                     UIAP_TOP_LEFT, "\0", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[2], 2, UIET_Label, hud_killfeed_label_b);

  as = malloc(1 * sizeof(int));
  as[0] = 0;

  font = TTF_OpenFont("fonts/jersey25.ttf", 20);

  UILabel *hud_killfeed_label_c =
      uilabel_create(0.03, 0.09, 0.5, 0.5, UIAP_TOP_LEFT, as, 1,
                     UIAP_TOP_LEFT, "\0", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[2], 3, UIET_Label, hud_killfeed_label_c);

  UILabel **hud_killfeed_labels = malloc(3 * sizeof(UILabel *));
  hud_killfeed_labels[0] = hud_killfeed_label_a;
  hud_killfeed_labels[1] = hud_killfeed_label_b;
  hud_killfeed_labels[2] = hud_killfeed_label_c;
  UIFeed *hud_killfeed_feed = uifeed_create(hud_killfeed_labels, 3, 5000);

  uimodule_set_element(modules[2], 0, UIET_Feed, hud_killfeed_feed);

  // INFO: Module 3, chat

  modules[3] = uimodule_create(3, 6);

  // label
  as = malloc(1 * sizeof(int));
  as[0] = 0;

  font = TTF_OpenFont("fonts/jersey25.ttf", 20);

  UILabel *hud_chat_label_a =
      uilabel_create(0.01, 0.8, 0.5, 0.1, UIAP_BOTTOM_LEFT, as, 1,
                     UIAP_BOTTOM_LEFT, "\0", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[3], 0, UIET_Label, hud_chat_label_a);

  // label
  as = malloc(1 * sizeof(int));
  as[0] = 0;

  font = TTF_OpenFont("fonts/jersey25.ttf", 20);

  UILabel *hud_chat_label_b =
      uilabel_create(0.01, 0.76, 0.5, 0.1, UIAP_BOTTOM_LEFT, as, 1,
                     UIAP_BOTTOM_LEFT, "\0", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[3], 1, UIET_Label, hud_chat_label_b);

  // label
  as = malloc(1 * sizeof(int));
  as[0] = 0;

  font = TTF_OpenFont("fonts/jersey25.ttf", 20);

  UILabel *hud_chat_label_c =
      uilabel_create(0.01, 0.72, 0.5, 0.1, UIAP_BOTTOM_LEFT, as, 1,
                     UIAP_BOTTOM_LEFT, "\0", 0xFF, 0xFF, 0xFF, 0xFF, font);

  uimodule_set_element(modules[3], 2, UIET_Label, hud_chat_label_c);

  // label
  as = malloc(1 * sizeof(int));
  as[0] = 0;

  font = TTF_OpenFont("fonts/jersey25.ttf", 20);

  SDL_Color bg = {.r = 0x00, .g = 0x00, .b = 0x00, .a = 0x00};
  SDL_Color border = {.r = 0x88, .g = 0x88, .b = 0x88, .a = 0x88};

  UITextBox *hud_chat_tb = uitextbox_create(0.01, 0.84, 0.5, 0.035, UIAP_BOTTOM_LEFT, as, 1, font, UIAP_BOTTOM_LEFT, 128, bg, bg, border, SDL_SCANCODE_RSHIFT);

  uimodule_set_element(modules[3], 3, UIET_Textbox, hud_chat_tb);

  // label
  as = malloc(1 * sizeof(int));
  as[0] = 0;

  UIButton *hud_chat_sender = uibutton_create(0, 0, 0, 0, UIAP_BOTTOM_RIGHT, as, 1, bg, bg, bg, bg, UIEC_SendChat, SDL_SCANCODE_RETURN);

  uimodule_set_element(modules[3], 4, UIET_Button, hud_chat_sender);

  UILabel **hud_chat_labels = malloc(3 * sizeof(UILabel *));
  hud_chat_labels[0] = hud_chat_label_a;
  hud_chat_labels[1] = hud_chat_label_b;
  hud_chat_labels[2] = hud_chat_label_c;

  UIFeed *hud_chat_feed = uifeed_create(hud_chat_labels, 3, 10000);

  uimodule_set_element(modules[3], 5, UIET_Feed, hud_chat_feed);

  // INFO: Module 4: Scoreboard

  modules[4] = uimodule_create(4, 4);

  as = malloc(1 * sizeof(int));
  as[0] = 1;

  font = TTF_OpenFont("fonts/jersey25.ttf", 40);
  
  UILabel *sb_1st = uilabel_create(0.1, 0.1, 0.8, 0.1, UIAP_TOP_LEFT, as, 1, UIAP_BOTTOM_LEFT, " -- empty slot -- ", 0xff, 0xff, 0xff, 0xff, font);

  uimodule_set_element(modules[4], 0, UIET_Label, sb_1st);

  as = malloc(1 * sizeof(int));
  as[0] = 1;

  font = TTF_OpenFont("fonts/jersey25.ttf", 40);

  UILabel *sb_2nd = uilabel_create(0.1, 0.2, 0.8, 0.1, UIAP_TOP_LEFT, as, 1, UIAP_BOTTOM_LEFT, " -- empty slot -- ", 0xff, 0xff, 0xff, 0xff, font);

  uimodule_set_element(modules[4], 1, UIET_Label, sb_2nd);

  as = malloc(1 * sizeof(int));
  as[0] = 1;

  font = TTF_OpenFont("fonts/jersey25.ttf", 40);

  UILabel *sb_3rd = uilabel_create(0.1, 0.3, 0.8, 0.1, UIAP_TOP_LEFT, as, 1, UIAP_BOTTOM_LEFT, " -- empty slot -- ", 0xff, 0xff, 0xff, 0xff, font);

  uimodule_set_element(modules[4], 2, UIET_Label, sb_3rd);

  as = malloc(1 * sizeof(int));
  as[0] = 1;

  font = TTF_OpenFont("fonts/jersey25.ttf", 40);

  UILabel *sb_4th = uilabel_create(0.1, 0.4, 0.8, 0.1, UIAP_TOP_LEFT, as, 1, UIAP_BOTTOM_LEFT, " -- empty slot -- ", 0xff, 0xff, 0xff, 0xff, font);

  uimodule_set_element(modules[4], 3, UIET_Label, sb_4th);

  return modules;
}

void free_ui(UIModule **yay, int nuimodules)
{
  for (int i = 0; i < nuimodules; i++)
  {
    uimodule_free(yay[i]);
  }
  free(yay);
}
