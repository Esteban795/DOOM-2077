#ifndef _LIB_DOOM_SYSTEM_SERVER_APPLY_EVENT_H
#define _LIB_DOOM_SYSTEM_SERVER_APPLY_EVENT_H

#include "../../ecs/world.h"
#include "../../event/event.h"

extern const system_t APPLY_EVENT_SYSTEM;

/*
* The first system in the chain, it applies the modifications to the world based on the event.
* For example, if the event is a player moving, this system will update the player's position in the world.
* This system is intented to be used on the server side, and ensure the world and events are consistent.
*/
int apply_event(world_t* world, event_t* event);

#endif