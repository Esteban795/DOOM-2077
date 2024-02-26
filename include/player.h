#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>

#include "geometry.h"
#include "structs.h"

double deg_to_rad(double deg);

player *player_init(engine *e);

void update_player(player *p, int mouse_x, const uint8_t *keyboard_state);

void player_free(player *p);
#endif