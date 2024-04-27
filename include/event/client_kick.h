#ifndef _LIB_DOOM_EVENT_CLIENTKICK_H
#define _LIB_DOOM_EVENT_CLIENTKICK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

#define CLIENT_PLAYER_KICK_EVENT_TAG 0x0003

// client_player_kick_event_t is an event that is triggered when a player is kicked by the server.
typedef struct {
    uint16_t tag;
    // entity_id of the player that has been kicked by the server.
    uint64_t entity_id;
} client_player_kick_event_t;

// ClientPlayerKickEvent_new creates a new ClientPlayerKickEvent.
client_player_kick_event_t* ClientPlayerKickEvent_new(uint64_t entity_id);

#endif