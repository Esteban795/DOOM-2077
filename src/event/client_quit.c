#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/event/client_quit.h"

client_player_quit_event_t* client_player_quit_event_new(uint64_t entity_id) {
    client_player_quit_event_t* e = malloc(sizeof(client_player_quit_event_t));
    e->tag = CLIENT_PLAYER_QUIT_EVENT_TAG;
    e->entity_id = entity_id;
    return e;
}