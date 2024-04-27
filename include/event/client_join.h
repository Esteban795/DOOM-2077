#ifndef _LIB_DOOM_EVENT_CLIENTJOIN_H
#define _LIB_DOOM_EVENT_CLIENTJOIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

#define CLIENT_PLAYER_JOIN_EVENT_TAG 0x0001

// client_player_join_event_t is an event that is triggered when a player joins the server.
typedef struct {
    uint16_t tag;
    // name of the player that joined the server.
    char* name;
    // entity_id of the player that joined the server.
    uint64_t entity_id;
} client_player_join_event_t;

// ClientPlayerJoinEvent_new creates a new ClientPlayerJoinEvent.
client_player_join_event_t* ClientPlayerJoinEvent_new(char* name, uint64_t entity_id);

#endif