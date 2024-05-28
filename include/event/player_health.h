#ifndef _LIB_DOOM_EVENT_PLAYERHEALTH_H
#define _LIB_DOOM_EVENT_PLAYERHEALTH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

/// Event tag for the player health event.
#define CLIENT_PLAYER_HEALTH_EVENT_TAG 0x0043
/// Event tag for the server player health event.
#define SERVER_PLAYER_HEALTH_EVENT_TAG (CLIENT_PLAYER_HEALTH_EVENT_TAG | 0x8000)

/// player_health_event_t is an event that is triggered when the health of a player is updated.  
/// It might be triggered when a player loses health, or when a player gains health, but 
/// generally is not triggered by a in-game event, but by the game logic (e.g. respawn).
typedef struct {
    /// The tag of the event.  
    /// This should be CLIENT_PLAYER_HEALTH_EVENT_TAG or SERVER_PLAYER_HEALTH_EVENT_TAG
    uint16_t tag;
    /// entity_id of the player
    uint64_t entity_id;
    /// total health of the player
    float hp;
    /// max health of the player
    float max_hp;
} player_health_event_t;

/// ClientPlayerHealthEvent creates a new ClientPlayerHealthEvent.
player_health_event_t* ClientPlayerHealthEvent_new(uint64_t entity_id, float hp, float max_hp);

//// ServerPlayerHealthEvent creates a new ServerPlayerHealthEvent.
player_health_event_t* ServerPlayerHealthEvent_new(uint64_t entity_id, float hp, float max_hp);

#endif