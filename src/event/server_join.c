#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/event/server_join.h"

server_player_join_event_t* ServerPlayerJoinEvent_new(uint64_t entity_id, char* name) {
    server_player_join_event_t* e = malloc(sizeof(server_player_join_event_t));
    e->tag = SERVER_PLAYER_JOIN_EVENT_TAG;
    strncpy(e->name, name, 127);
    e->name[127] = '\0';
    e->entity_id = entity_id;
    return e;
}