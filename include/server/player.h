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

entity_t* server_create_player(world_t* world, char* name);

#endif