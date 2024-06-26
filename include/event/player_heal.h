#ifndef _LIB_DOOM_EVENT_PLAYERHEAL_H
#define _LIB_DOOM_EVENT_PLAYERHEAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

/// Event tag for the player heal event.
#define CLIENT_PLAYER_HEAL_EVENT_TAG 0x0042
/// Event tag for the server player heal event.
#define SERVER_PLAYER_HEAL_EVENT_TAG (CLIENT_PLAYER_HEAL_EVENT_TAG | 0x8000)

/// player_heal_event_t is an event that is triggered when a player gains health.  
/// It might be triggered by natural regeneration, or by a player being healed by another player.
typedef struct {
    /// The tag of the event.  
    /// This should be CLIENT_PLAYER_HEAL_EVENT_TAG or SERVER_PLAYER_HEAL_EVENT_TAG
    uint16_t tag;
    /// entity_id of the player that earns health.
    uint64_t entity_id;
    /// gain in health points.
    float gain;
} player_heal_event_t;

/// ClientPlayerHealEvent_new creates a new ClientPlayerHealEvent.
player_heal_event_t* ClientPlayerHealEvent_new(uint64_t entity_id, float gain);

/// ServerPlayerHealEvent_new creates a new ServerPlayerHealEvent.
player_heal_event_t* ServerPlayerHealEvent_new(uint64_t entity_id, float gain);

#endif