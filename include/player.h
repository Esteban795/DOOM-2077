#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>

#include "geometry.h"
#include "structs.h"

double deg_to_rad(double deg);

player *player_init(engine *e);

void update_player(player *p, int mouse_x, const uint8_t *keyboard_state);

void player_free(player *p);

void players_free(player** players, int num_players);

player** create_players(int num_players,engine* e);

void update_height(player* p,double z);
#endif
