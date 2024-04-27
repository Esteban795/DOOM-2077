#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/event/client_join.h"

client_player_join_event_t* client_player_join_event_new(char* name, uint64_t entity_id) {
    client_player_join_event_t* e = malloc(sizeof(client_player_join_event_t));
    e->tag = CLIENT_PLAYER_JOIN_EVENT_TAG;
    e->name = malloc(strlen(name) + 1);
    strcpy(e->name, name);
    e->entity_id = entity_id;
    return e;
}