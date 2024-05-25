#ifndef _LIB_DOOM_EVENT_PLAYERDAMAGE_H
#define _LIB_DOOM_EVENT_PLAYERDAMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

/// Event tag for the player damage event.
#define CLIENT_PLAYER_DAMAGE_EVENT_TAG 0x0041
/// Event tag for the server player damage event.
#define SERVER_PLAYER_DAMAGE_EVENT_TAG (CLIENT_PLAYER_DAMAGE_EVENT_TAG | 0x8000)

/// player_damage_event_t is an event that is triggered when a player loses health.  
/// It might be triggered when a player is hit by a projectile, or when the player falls from a high place.  
/// If the damage is caused by another player, the option source_entity_id should be set to the entity_id of 
/// the player that caused the damage. Otherwise it is set to 0.
///
/// \important This event is not triggered when a player is killed, even though the player is damaged when killed.
/// For that, use the player_kill_event_t event.
typedef struct {
    /// The tag of the event.  
    /// This should be CLIENT_PLAYER_DAMAGE_EVENT_TAG or SERVER_PLAYER_DAMAGE_EVENT_TAG
    uint16_t tag;
    /// entity_id of the player that lost health.
    uint64_t entity_id;
    /// entity_id of the player that caused the damage.  
    /// 0 if the damage was not caused by another player.
    uint64_t source_entity_id;
    /// damage in health points.
    int damage;
} player_damage_event_t;

/// ClientPlayerDamageEvent creates a new ClientPlayerDamageEvent.
///
/// \param entity_id entity_id of the player that lost health.
/// \param source_entity_id entity_id of the player that caused the damage.
///                         0 if the damage was not caused by another player.
/// \param damage damage in health points.
player_damage_event_t* ClientPlayerDamageEvent_new(uint64_t entity_id, uint64_t source_entity_id, int damage);

/// ServerPlayerDamageEvent_new creates a new ServerPlayerDamageEvent.
///
/// \param entity_id entity_id of the player that lost health.
/// \param source_entity_id entity_id of the player that caused the damage.
///                         0 if the damage was not caused by another player.
/// \param damage damage in health points.
player_damage_event_t* ServerPlayerDamageEvent_new(uint64_t entity_id,  uint64_t source_entity_id, int damage);

#endif