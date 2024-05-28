#ifndef _LIB_DOOM_SYSTEM_SERVER_BROADCAST_EVENT_H
#define _LIB_DOOM_SYSTEM_SERVER_BROADCAST_EVENT_H

#include "../../ecs/world.h"
#include "../../event/event.h"

extern const system_t BROADCAST_EVENT_SYSTEM;

/**
* Generally the last system in the chain, the broadcast_event_system will broadcast the event to all the clients.  
* \note This system is intented to be used on the server side.
*/
int broadcast_event(world_t* world, event_t* event);

#endif