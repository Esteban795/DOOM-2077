#ifndef WEAPONS_H
#define WEAPONS_H

#include "structs.h"

weapons_array* init_weapons_array();
void switch_weapon(player* p, int weapon_id);
void free_weapons_array(weapons_array* wa);
#endif