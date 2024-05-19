#ifndef _LIB_DOOM_EVENT_SCOREBOARDUPDATE_H
#define _LIB_DOOM_EVENT_SCOREBOARDUPDATE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "event.h"
#include "../ecs/world.h"

#define CLIENT_SCOREBOARD_UPDATE_EVENT_TAG 0x00c2
#define SERVER_SCOREBOARD_UPDATE_EVENT_TAG (CLIENT_SCOREBOARD_UPDATE_EVENT_TAG | 0x8000)

// player_move_event_t is an event that is triggered when a player moves.
typedef struct {
    uint16_t tag;
    // number of entries in the scoreboard.
    uint16_t entries_count;
    // entries' names.
    char entries[10][128];
    // entries' deaths.
    uint16_t deaths[10];
    // entries' kills.
    uint16_t kills[10];
} scoreboard_update_event_t;

// ClientScoreboardUpdateEvent_new creates a new ClientScoreboardUpdateEvent.
scoreboard_update_event_t* ClientScoreboardUpdateEvent_new(uint16_t entries_count, char** entries, uint16_t* deaths, uint16_t* kills);

// ServerScoreboardUpdateEvent_new creates a new ServerScoreboardUpdateEvent.
scoreboard_update_event_t* ServerScoreboardUpdateEvent_new(uint16_t entries_count, char** entries, uint16_t* deaths, uint16_t* kills);

// Generate the scoreboard for the world.
// Objective: generate a scoreboard broadcastable as an event.
// Returns the number of entries in the generated scoreboard.
int scoreboard_generate(world_t* world, char* entries[10], uint16_t deaths[10], uint16_t kills[10]);

#endif