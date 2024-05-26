#ifndef PLAYER_H
#define PLAYER_H

#include <math.h>

#include "geometry.h"
#include "structs.h"
#include "events.h"
#include "node.h"
#include "weapons.h"
#include "component/position.h"
#include "component/health.h"
#include "component/weapon.h"
#include "component/subsector_id.h"

#ifndef PLAYER_USERNAME
#define PLAYER_USERNAME "player"
#endif
#include "settings.h"

#define PLAYER_HITBOX_SIZE 50
#define HITSCAN_PRECISION 10
#define WEAPON_AMMO_CAPACITY 10

player *player_init(engine *e);

void update_player(player *p);

void player_free(player *p);

void update_height(player *p);

void process_keys(player* p);

// Get the player's position component
position_ct* player_get_position(player* p);

// Get the player's health component
health_ct* player_get_health(player* p);

// Get the player's weapon component
weapon_ct* player_get_weapon(player* p);

// Find the player index in the player list.
int player_find(entity_t** list, entity_t* p);

// Find a player by id in the player list.
int player_find_by_id(entity_t** list, uint64_t id);


void free_cooldowns_sprays(player* p);

WACS * create_cooldowns_sprays(player* p);

WACS* update_cooldowns_sprays(WACS* w);

void fire_bullet(
    entity_t **players, int num_players, player *player_,
    weapons_array *weapons_list);
#endif
