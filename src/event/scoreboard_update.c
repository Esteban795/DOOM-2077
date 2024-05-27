#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/event/scoreboard_update.h"
#include "../../include/component/display_name.h"
#include "../../include/component/statistic.h"

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
        event->deaths[i] = deaths[i];
        event->kills[i] = kills[i];
    }
    return event;
}

scoreboard_update_event_t* ClientScoreboardUpdateEvent_new(uint16_t entries_count, char** entries, uint16_t* deaths, uint16_t* kills) {
    return scoreboard_update_event_new(CLIENT_SCOREBOARD_UPDATE_EVENT_TAG, entries_count, entries, deaths, kills);
}

scoreboard_update_event_t* ServerScoreboardUpdateEvent_new(uint16_t entries_count, char** entries, uint16_t* deaths, uint16_t* kills) {
    return scoreboard_update_event_new(SERVER_SCOREBOARD_UPDATE_EVENT_TAG, entries_count, entries, deaths, kills);
}

int scoreboard_generate(world_t* world, char* entries[10], uint16_t deaths[10], uint16_t kills[10]) {
    // Get at most 10 players (the older ones).
    entity_t* players[10] = {0};
    int i = 0;
    int j = 0;
    while (j < 10 && i < (int) vec_length(&world->entities)) {
        entity_t* player = vec_get(&world->entities, i);
        if (!world_entity_has_component(world, player, COMPONENT_TAG_STATISTIC)) {
            i++;
            continue;
        }
        players[j] = player;
        j++;
        i++;
    }

    // Sort the players by kills.
    for (int i = 0; i < 10; i++) {
        if (players[i] == NULL) {
            continue;
        }
        for (int j = i + 1; j < 10; j++) {
            if (players[j] == NULL) {
                continue;
            }
            statistic_ct* stats_i = (statistic_ct*) world_get_component(world, players[i], COMPONENT_TAG_STATISTIC);
            statistic_ct* stats_j = (statistic_ct*) world_get_component(world, players[j], COMPONENT_TAG_STATISTIC);
            if (stats_i == NULL) {
                players[i] = players[j];
                players[j] = NULL;
            } else if (stats_j == NULL) {
                players[j] = NULL;
            } else if (statistic_get_kills(stats_i) < statistic_get_kills(stats_j)) {
                entity_t* tmp = players[i];
                players[i] = players[j];
                players[j] = tmp;
            }
        }
    }

    int count = 0;
    // Generate the scoreboard.
    for (int i = 0; i < 10; i++) {
        if (players[i] == NULL) {
            entries[i] = "";
            deaths[i] = 0;
            kills[i] = 0;
            continue;
        }
        display_name_ct* display_name = (display_name_ct*) world_get_component(world, players[i], COMPONENT_TAG_DISPLAY_NAME);
        statistic_ct* stats = (statistic_ct*) world_get_component(world, players[i], COMPONENT_TAG_STATISTIC);
        if (display_name == NULL || stats == NULL) {
            entries[i] = "";
            deaths[i] = 0;
            kills[i] = 0;
            continue;
        }
        entries[i] = display_name_get(display_name);
        deaths[i] = statistic_get_deaths(stats);
        kills[i] = statistic_get_kills(stats);
        count++;
    }
    return count;
}    