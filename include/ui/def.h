#ifndef UI_DEF_H
#define UI_DEF_H

#include "module.h"

UIModule** get_ui_ingame(int* nuimodules);
void free_ui_ingame(UIModule** o);

#endif