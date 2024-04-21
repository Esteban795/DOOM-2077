#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>

#include "geometry.h"
#include "structs.h"
#include "events.h"
#include "component/position.h"
#include "component/health.h"
#include "component/weapon.h"

double deg_to_rad(double deg);

player *player_init(engine *e);

void update_player(player *p);

void player_free(player *p);

void update_height(player* p,double z);

// Get the player's position component
position_ct* player_get_position(player* p);

// Get the player's health component
health_ct* player_get_health(player* p);

// Get the player's weapon component
weapon_ct* player_get_weapon(player* p);

#endif
