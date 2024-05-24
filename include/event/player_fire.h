#ifndef _LIB_DOOM_EVENT_PLAYERFIRE_H
#define _LIB_DOOM_EVENT_PLAYERFIRE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

#define CLIENT_PLAYER_FIRE_EVENT_TAG 0x0045
#define SERVER_PLAYER_FIRE_EVENT_TAG (CLIENT_PLAYER_FIRE_EVENT_TAG | 0x8000)

// player_fire_event_t is an event that is triggered when a player fires a weapon.
// It contains the entity_id of the player that fired the weapon, and the weapon_id of the weapon fired.
typedef struct {
    uint16_t tag;
    // entity_id of the player
    uint64_t entity_id;
    // id of the weapon fired
    int8_t weapon_id;
} player_fire_event_t;

// ClientPlayerFireEvent creates a new ClientPlayerFireEvent.
player_fire_event_t* ClientPlayerFireEvent_new(uint64_t entity_id, int8_t weapon_id);

// ServerPlayerFireEvent creates a new ServerPlayerFireEvent.
player_fire_event_t* ServerPlayerFireEvent_new(uint64_t entity_id, int8_t weapon_id);

#endif