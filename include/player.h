#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>

#include "geometry.h"
#include "structs.h"
#include "events.h"

double deg_to_rad(double deg);

player *player_init(engine *e);

void update_player(player *p);

void player_free(player *p);

void players_free(player** players, int num_players);

player** create_players(int num_players,engine* e);

void update_height(player* p,double z);

void free_cooldowns_sprays(player* p);

WACS * create_cooldowns_sprays(player* p);

WACS* update_cooldowns_sprays(WACS* w);
#endif
