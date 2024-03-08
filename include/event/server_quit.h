#define _LIB_DOOM_EVENT_SERVERQUIT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifndef _LIB_DOOM_EVENT_EVENT_H
#include "event.h"
#endif

#define SERVER_PLAYER_QUIT_EVENT_TAG 0x8002

// ServerPlayerQuitEvent is an event that is triggered when a player quits the server.
typedef struct {
    uint16_t tag;
    // name of the player that left the server.
    char* name;
} ServerPlayerQuitEvent;

// ServerPlayerQuitEvent_new creates a new ServerPlayerQuitEvent.
ServerPlayerQuitEvent* ServerPlayerQuitEvent_new(char* name);
