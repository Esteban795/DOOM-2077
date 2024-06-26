#ifndef WEAPONS_H
#define WEAPONS_H

#define IDLE 0
#define ANIMATION 1
#define FIRE 2

#include "../structs.h"
#include "events.h"

extern weapons_array *wa;
#define WEAPONS_NUMBER 4

weapons_array *init_weapons_array(engine *e);
void switch_weapon(player *p, int weapon_id);
void free_weapons_array(weapons_array *wa);
void add_weapon(player *p, int weapon_id, weapons_array *wa);
void add_ammo(player *p, int weapon_id, int ammo);
void print_animations_patches(weapon *w);
// void draw_weapon(map_renderer *mr, patch sprite, int x, int y);
// void idle_weapon_animation(map_renderer *mr,weapon *w);
void update_animation(engine *e);
void update_weapons(engine *e);

#endif