#ifndef WEAPONS_H
#define WEAPONS_H

#define IDLE 0
#define FIRE 1

#include "structs.h"


weapons_array* init_weapons_array(map_renderer *mr);
void switch_weapon(player* p, int weapon_id);
void free_weapons_array(weapons_array* wa);
void add_weapon(player* p, int weapon_id    ,weapons_array* wa);
void add_ammo(player* p, int weapon_id, int ammo);
void print_animations_patches(weapon *w);

#endif