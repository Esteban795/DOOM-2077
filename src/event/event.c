#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifndef _LIB_DOOM_EVENT_EVENT_H
#include "../../include/event/event.h"
#endif

bool is_server_event(event_t* e) {
    return (e->tag & 0x8000) > 0;
}

bool is_client_event(event_t* e) {
    return (e->tag & 0x8000) == 0;
}