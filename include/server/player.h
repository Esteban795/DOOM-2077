#ifndef _LIB_DOOM_SERVER_PLAYER_H
#define _LIB_DOOM_SERVER_PLAYER_H

#include "state.h"
#include "../component/display_name.h"
#include "../component/health.h"
#include "../component/position.h"
#include "../component/weapon.h"
#include "../component/statistic.h"
#include "../ecs/world.h"
#include "../ecs/entity.h"
#include "../ecs/component.h"

/// @brief Create a player entity.
///
/// This function creates a player entity with the given name and adds it to the world.
/// The player entity will have the following components:
/// * display_name_t
/// * health_t
/// * position_t
/// * weapon_t
/// * statistic_t
entity_t* server_create_player(world_t* world, char* name);

#endif