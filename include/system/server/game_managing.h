#ifndef _LIB_DOOM_SYSTEM_SERVER_GAME_MANAGING_H
#define _LIB_DOOM_SYSTEM_SERVER_GAME_MANAGING_H

#include "../../ecs/world.h"
#include "../../event/event.h"

/// Minimum required players to start a game.
#define SERVER_MINIMUM_PLAYERS 2
/// Game duration in seconds.
#define SERVER_GAME_DURATION 600 // in seconds
/// Cooldown before restarting the game in seconds.
#define SERVER_GAME_COOLDOWN_BEFORE_RESTART 45 // in seconds

extern const system_t GAME_MANAGING_SYSTEM;

/**
* Game managing system, this system is responsible for managing the launch and end of the game.
*/
int game_managing(world_t* world, event_t* event);

#endif