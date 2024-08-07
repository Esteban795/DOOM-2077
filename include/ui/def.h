#ifndef UI_DEF_H
#define UI_DEF_H

#include "event_handler.h"
#include "module.h"
#include <stdbool.h>

#define SUBSTATE_MENU_MAIN 0
#define SUBSTATE_MENU_CONNECT 1
#define SUBSTATE_MENU_SETTINGS 2

#define SUBSTATE_INGAME_PLAYING 0
#define SUBSTATE_INGAME_SCOREBOARD 1

UIModule **get_ui_menu(SDL_Renderer *r, int *nuimodules);
UIModule **get_ui_ingame(SDL_Renderer *r, int *nuimodules);
void free_ui(UIModule **o, int nuimodules);

#endif
