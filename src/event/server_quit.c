#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "../../include/event/server_quit.h"

server_player_quit_event_t* server_player_quit_event_new(char* name) {
    server_player_quit_event_t* e = malloc(sizeof(server_player_quit_event_t));
    e->tag = SERVER_PLAYER_QUIT_EVENT_TAG;
    e->name = malloc(strlen(name) + 1);
    strcpy(e->name, name);
    return e;
}