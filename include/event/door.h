#ifndef _LIB_DOOM_EVENT_DOOR_H
#define _LIB_DOOM_EVENT_DOOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "event.h"
#include "../ecs/world.h"
#include "../ecs/entity.h"
#include "../component/door_state.h"
#include "../component/lift_state.h"


#define CLIENT_DOOR_STATES_EVENT_TAG 0x0080
#define SERVER_DOOR_STATES_EVENT_TAG (CLIENT_DOOR_STATES_EVENT_TAG | 0x8000)
#define CLIENT_DOOR_OPEN_EVENT_TAG 0x0081
#define SERVER_DOOR_OPEN_EVENT_TAG (CLIENT_DOOR_OPEN_EVENT_TAG | 0x8000)
#define CLIENT_DOOR_CLOSE_EVENT_TAG 0x0082
#define SERVER_DOOR_CLOSE_EVENT_TAG (CLIENT_DOOR_CLOSE_EVENT_TAG | 0x8000)

// door_open_event_t represents a door open event or lift ascending event.
typedef struct {
    uint16_t tag;
    uint64_t door_id;
    bool is_lift;
} door_open_event_t;

// door_close_event_t represents a door close event or lift descending event.
typedef struct {
    uint16_t tag;
    uint64_t door_id;
    bool is_lift;
} door_close_event_t;

inline door_open_event_t* ClientDoorOpenEvent_new(uint64_t door_id, bool is_lift) {
    door_open_event_t *event = (door_open_event_t*)malloc(sizeof(door_open_event_t));
    event->tag = CLIENT_DOOR_OPEN_EVENT_TAG;
    event->door_id = door_id;
    event->is_lift = is_lift;
    return event;
}

inline door_close_event_t* ClientDoorCloseEvent_new(uint64_t door_id, bool is_lift) {
    door_close_event_t *event = (door_close_event_t*)malloc(sizeof(door_close_event_t));
    event->tag = CLIENT_DOOR_CLOSE_EVENT_TAG;
    event->door_id = door_id;
    event->is_lift = is_lift;
    return event;
}

inline door_open_event_t* ServerDoorOpenEvent_new(uint64_t door_id, bool is_lift) {
    door_open_event_t *event = (door_open_event_t*)malloc(sizeof(door_open_event_t));
    event->tag = SERVER_DOOR_OPEN_EVENT_TAG;
    event->door_id = door_id;
    event->is_lift = is_lift;
    return event;
}

inline door_close_event_t* ServerDoorCloseEvent_new(uint64_t door_id, bool is_lift) {
    door_close_event_t *event = (door_close_event_t*)malloc(sizeof(door_close_event_t));
    event->tag = SERVER_DOOR_CLOSE_EVENT_TAG;
    event->door_id = door_id;
    event->is_lift = is_lift;
    return event;
}

int door_states_generate(world_t* world, entity_t** doors, bool* doors_states, int doors_count);
int lift_states_generate(world_t* world, entity_t** lifts, bool* lifts_states, int lifts_count);

#endif