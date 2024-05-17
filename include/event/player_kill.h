#ifndef _LIB_DOOM_EVENT_PLAYERKILL_H
#define _LIB_DOOM_EVENT_PLAYERKILL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

#define CLIENT_PLAYER_KILL_EVENT_TAG 0x0044
#define SERVER_PLAYER_KILL_EVENT_TAG (CLIENT_PLAYER_KILL_EVENT_TAG | 0x8000)

// player_kill_event_t is an event that is triggered when a player is killed.
// It might be triggered when a player is hit by a projectile, or when the player falls from a high place.
// If the damage is caused by another player, the option source_entity_id should be set to the entity_id of 
// the player that caused the damage. Otherwise it is set to 0.
// This event is similar to player_damage_event_t, but it is only triggered when the player is killed. 
// When a player is killed, the player is also damaged, but the player_damage_event_t is not triggered.
typedef struct {
    uint16_t tag;
    // entity_id of the player that has been killed.
    uint64_t entity_id;
    // entity_id of the player that caused the damage.
    // 0 if the damage was not caused by another player.
    uint64_t source_entity_id;
} player_kill_event_t;

// ClientPlayerKillEvent_new creates a new ClientPlayerKillEvent.
player_kill_event_t* ClientPlayerKillEvent_new(uint64_t entity_id, uint64_t source_entity_id);

// ServerPlayerKillEvent_new creates a new ServerPlayerKillEvent.
player_kill_event_t* ServerPlayerKillEvent_new(uint64_t entity_id,  uint64_t source_entity_id);

#endif