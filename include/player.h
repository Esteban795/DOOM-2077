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

void update_height(player* p,double z);
#endif
