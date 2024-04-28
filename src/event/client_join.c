#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/event/client_join.h"

client_player_join_event_t* ClientPlayerJoinEvent_new(char* name, uint64_t entity_id) {
    client_player_join_event_t* e = malloc(sizeof(client_player_join_event_t));
    e->tag = CLIENT_PLAYER_JOIN_EVENT_TAG;
    strncpy(e->name, name, 127);
    e->name[127] = '\0';
    e->entity_id = entity_id;
    return e;
}