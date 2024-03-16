#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifndef _LIB_DOOM_EVENT_SERVERJOIN_H
#include "../../include/event/server_join.h"
#endif

server_player_join_event_t* server_player_join_event_new(char* name) {
    server_player_join_event_t* e = malloc(sizeof(server_player_join_event_t));
    e->tag = SERVER_PLAYER_JOIN_EVENT_TAG;
    e->name = malloc(strlen(name) + 1);
    strcpy(e->name, name);
    return e;
}