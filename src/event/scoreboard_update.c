#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/event/scoreboard_update.h"

scoreboard_update_event_t* scoreboard_update_event_new(uint16_t tag, uint16_t entries_count, char** entries, uint16_t* deaths, uint16_t* kills) {
    scoreboard_update_event_t* event = malloc(sizeof(scoreboard_update_event_t));
    event->tag = tag;
    if (entries_count > 10) {
        entries_count = 10;
    }
    event->entries_count = entries_count;
    for (int i = 0; i < entries_count; i++) {
        strncpy(event->entries[i], entries[i], 127);
        event->entries[i][127] = '\0';
    }
    event->deaths = deaths;
    event->kills = kills;
    return event;
}

scoreboard_update_event_t* ClientScoreboardUpdateEvent_new(uint16_t entries_count, char** entries, uint16_t* deaths, uint16_t* kills) {
    return scoreboard_update_event_new(CLIENT_SCOREBOARD_UPDATE_EVENT_TAG, entries_count, entries, deaths, kills);
}

scoreboard_update_event_t* ServerScoreboardUpdateEvent_new(uint16_t entries_count, char** entries, uint16_t* deaths, uint16_t* kills) {
    return scoreboard_update_event_new(SERVER_SCOREBOARD_UPDATE_EVENT_TAG, entries_count, entries, deaths, kills);
}