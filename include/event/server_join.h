#ifndef _LIB_DOOM_EVENT_SERVERJOIN_H
#define _LIB_DOOM_EVENT_SERVERJOIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

#define SERVER_PLAYER_JOIN_EVENT_TAG 0x8001

// server_player_join_event_t is an event that is triggered when a player joins the server.
typedef struct {
    uint16_t tag;
    // entity_id of the player that joined the server.
    uint64_t entity_id;
    // name of the player that joined the server.
    char name[128];
} server_player_join_event_t;

// ServerPlayerJoinEvent_new creates a new ServerPlayerJoinEvent.
server_player_join_event_t* ServerPlayerJoinEvent_new(uint64_t entity_id, char* name);

#endif