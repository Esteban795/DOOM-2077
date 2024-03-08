#define _LIB_DOOM_EVENT_SERVERJOIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifndef _LIB_DOOM_EVENT_EVENT_H
#include "event.h"
#endif

#define SERVER_PLAYER_JOIN_EVENT_TAG 0x8001

// ServerPlayerJoinEvent is an event that is triggered when a player joins the server.
typedef struct {
    uint16_t tag;
    // name of the player that joined the server.
    char* name;
} ServerPlayerJoinEvent;

// ServerPlayerJoinEvent_new creates a new ServerPlayerJoinEvent.
ServerPlayerJoinEvent* ServerPlayerJoinEvent_new(char* name);
