#ifndef _LIB_DOOM_EVENT_SERVERQUIT_H
#define _LIB_DOOM_EVENT_SERVERQUIT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "event.h"

/// Event tag for the server player quit event.
#define SERVER_PLAYER_QUIT_EVENT_TAG 0x8002

/// server_player_quit_event_t is an event that is triggered when a player quits the server.
typedef struct {
    /// The tag of the event.  
    /// This should be SERVER_PLAYER_QUIT_EVENT_TAG
    uint16_t tag;
    /// entity_id of the player that left the server.
    uint64_t entity_id;
    /// name of the player that left the server.
    ///
    /// \remark The name should be at most 127 characters long, and NUL-terminated.
    char name[128];
} server_player_quit_event_t;

/// ServerPlayerQuitEvent_new creates a new ServerPlayerQuitEvent.
server_player_quit_event_t* ServerPlayerQuitEvent_new(uint64_t entity_id, char* name);

#endif