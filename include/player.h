#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>

#include "geometry.h"
#include "structs.h"
#include "events.h"
#include "settings.h"

#define PLAYER_HITBOX_SIZE 50
#define HITSCAN_PRECISION 10

double deg_to_rad(double deg);

player *player_init(engine *e);

void update_player(player *p);

void player_free(player *p);

void players_free(player** players, int num_players);

player** create_players(int num_players,engine* e);

void update_height(player *p);

void process_keys(player* p);
#endif
