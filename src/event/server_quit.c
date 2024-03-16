#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifndef _LIB_DOOM_EVENT_SERVERQUIT_H
#include "../../include/event/server_quit.h"
#endif

server_player_quit_event_t* server_player_quit_event_new(char* name) {
    server_player_quit_event_t* e = malloc(sizeof(server_player_quit_event_t));
    e->tag = SERVER_PLAYER_QUIT_EVENT_TAG;
    e->name = malloc(strlen(name) + 1);
    strcpy(e->name, name);
    return e;
}