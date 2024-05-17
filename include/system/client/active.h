#ifndef _LIB_DOOM_SYSTEM_CLIENT_ACTIVE_H
#define _LIB_DOOM_SYSTEM_CLIENT_ACTIVE_H

#include "../../ecs/world.h"
#include "apply_event.h"

// world_register_active_systems registers the active systems on the client side.
// Be sure to call this function once the world is initialized.
// NOTE: The order of registration matters. The first system registered will be the first to run, etc.
inline void world_register_active_systems(world_t* world) {
    world_register_system(world, APPLY_EVENT_SYSTEM.fn);
}

#endif