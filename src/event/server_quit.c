#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifndef _LIB_DOOM_EVENT_SERVERQUIT_H
#include "../../include/event/server_quit.h"
#endif

ServerPlayerQuitEvent* ServerPlayerQuitEvent_new(char* name) {
    ServerPlayerQuitEvent* e = malloc(sizeof(ServerPlayerQuitEvent));
    e->tag = SERVER_PLAYER_QUIT_EVENT_TAG;
    e->name = malloc(strlen(name) + 1);
    strcpy(e->name, name);
    return e;
}