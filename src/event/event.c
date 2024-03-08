#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#ifndef _LIB_DOOM_EVENT_EVENT_H
#include "../../include/event/event.h"
#endif

bool is_server_event(Event* e) {
    return (e->tag & 0x8000) > 0;
}

bool is_client_event(Event* e) {
    return (e->tag & 0x8000) == 0;
}