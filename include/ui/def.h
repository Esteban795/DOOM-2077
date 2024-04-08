#ifndef UI_DEF_H
#define UI_DEF_H

#include "module.h"

UIModule **get_ui_ingame(SDL_Renderer *r, int *nuimodules);
void free_ui_ingame(UIModule **o);

#endif
