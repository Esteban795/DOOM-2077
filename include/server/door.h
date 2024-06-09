#ifndef _LIB_DOOM_SERVER_DOOR_H
#define _LIB_DOOM_SERVER_DOOR_H

#include "state.h"
#include "../component/door_state.h"
#include "../component/lift_state.h"
#include "../ecs/world.h"
#include "../ecs/entity.h"
#include "../ecs/component.h"
#include "../WAD/wad_data.h"

/// Load doors from the WAD data and add them to the world as entity.
///
/// Returns the entities on success, NULL on failure. door_count is set to the number of doors.
entity_t** server_world_load_doors(world_t* world, wad_data* wad, int* door_count);

/// Load lifts from the WAD data and add them to the world as entity.
///
/// Returns the entities on success, NULL on failure. lift_count is set to the number of lifts.
entity_t** server_world_load_lifts(world_t* world, wad_data* wad, int* lift_count);

/// Unload doors from the world.
void server_world_unload_doors(world_t* world, entity_t** doors, int door_count);

/// Unload lifts from the world.
void server_world_unload_lifts(world_t* world, entity_t** lifts, int lift_count);

#endif