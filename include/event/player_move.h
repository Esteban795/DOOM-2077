#ifndef _LIB_DOOM_EVENT_PLAYERMOVE_H
#define _LIB_DOOM_EVENT_PLAYERMOVE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

/// Event tag for the client player move event.
#define CLIENT_PLAYER_MOVE_EVENT_TAG 0x0040
/// Event tag for the server player move event.
#define SERVER_PLAYER_MOVE_EVENT_TAG (CLIENT_PLAYER_MOVE_EVENT_TAG | 0x8000)

/// player_move_event_t is an event that is triggered when a player moves.
typedef struct {
    /// The tag of the event.  
    /// This should be CLIENT_PLAYER_MOVE_EVENT_TAG or SERVER_PLAYER_MOVE_EVENT_TAG
    uint16_t tag;
    /// entity_id of the player.
    uint64_t entity_id;
    /// x position of the player.
    double x;
    /// y position of the player.
    double y;
    /// z position of the player.
    double z;
    /// angle (in degrees) of the player.
    double angle;
} player_move_event_t;

/// ClientPlayerMoveEvent_new creates a new ClientPlayerMoveEvent.
player_move_event_t* ClientPlayerMoveEvent_new(uint64_t entity_id, double x, double y, double z, double angle);

/// ServerPlayerMoveEvent_new creates a new ServerPlayerMoveEvent.
player_move_event_t* ServerPlayerMoveEvent_new(uint64_t entity_id, double x, double y, double z, double angle);

#endif