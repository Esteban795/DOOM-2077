#ifndef _LIB_DOOM_EVENT_PLAYERWEAPONUPDATE_H
#define _LIB_DOOM_EVENT_PLAYERWEAPONUPDATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "event.h"
#include "../component/weapon.h"

/// CLIENT_PLAYER_WEAPON_UPDATE_EVENT_TAG is the tag of the client event that is triggered when the weapons of a player are updated.
#define CLIENT_PLAYER_WEAPON_UPDATE_EVENT_TAG 0x0046
/// SERVER_PLAYER_WEAPON_UPDATE_EVENT_TAG is the tag of the server event that is triggered when the weapons of a player are updated.
#define SERVER_PLAYER_WEAPON_UPDATE_EVENT_TAG (CLIENT_PLAYER_WEAPON_UPDATE_EVENT_TAG | 0x8000)

/// player_weapon_update_event_t is an event that is triggered when the weapons of a player are updated.  
/// It contains the entity_id of the player, and the ammunitions, mags and cooldowns of the player.
typedef struct {
    /// tag of the event
    /// Should be CLIENT_PLAYER_WEAPON_UPDATE_EVENT_TAG or SERVER_PLAYER_WEAPON_UPDATE_EVENT_TAG
    uint16_t tag;
    /// entity_id of the player
    uint64_t entity_id;
    /// ammunitions of the player
    int ammunitions[WEAPONS_NUMBER];
    /// mags of the player
    int mags[WEAPONS_NUMBER];
    /// cooldown by weapon of the player
    int cooldowns[WEAPONS_NUMBER];
} player_weapon_update_event_t;

// ClientPlayerWeaponUpdateEvent creates a new ClientPlayerWeaponUpdateEvent.
player_weapon_update_event_t* ClientPlayerWeaponUpdateEvent_new(uint64_t entity_id, int ammunitions[WEAPONS_NUMBER], int mags[WEAPONS_NUMBER], int cooldowns[WEAPONS_NUMBER]);

// ServerPlayerWeaponUpdateEvent creates a new ServerPlayerWeaponUpdateEvent.
player_weapon_update_event_t* ServerPlayerWeaponUpdateEvent_new(uint64_t entity_id, int ammunitions[WEAPONS_NUMBER], int mags[WEAPONS_NUMBER], int cooldowns[WEAPONS_NUMBER]);

#endif