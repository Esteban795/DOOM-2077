#ifndef _LIB_DOOM_EVENT_CLIENTQUIT_H
#define _LIB_DOOM_EVENT_CLIENTQUIT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

#define CLIENT_PLAYER_QUIT_EVENT_TAG 0x0002

// client_player_quit_event_t is an event that is triggered when a player quits the server.
typedef struct {
    uint16_t tag;
    // entity_id of the player that quitted the server.
    uint64_t entity_id;
} client_player_quit_event_t;

// ClientPlayerQuitEvent_new creates a new ClientPlayerQuitEvent.
client_player_quit_event_t* ClientPlayerQuitEvent_new(uint64_t entity_id);

#endif