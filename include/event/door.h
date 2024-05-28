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

/// Event tag for the client door states event.
#define CLIENT_DOOR_STATES_EVENT_TAG 0x0080
/// Event tag for the server door states event.
#define SERVER_DOOR_STATES_EVENT_TAG (CLIENT_DOOR_STATES_EVENT_TAG | 0x8000)
/// Event tag for the client open door event.
#define CLIENT_DOOR_OPEN_EVENT_TAG 0x0081
/// Event tag for the server open door event.
#define SERVER_DOOR_OPEN_EVENT_TAG (CLIENT_DOOR_OPEN_EVENT_TAG | 0x8000)
/// Event tag for the client close door event.
#define CLIENT_DOOR_CLOSE_EVENT_TAG 0x0082
/// Event tag for the server close door event.
#define SERVER_DOOR_CLOSE_EVENT_TAG (CLIENT_DOOR_CLOSE_EVENT_TAG | 0x8000)

/// door_open_event_t represents a door open event or lift ascending event.
typedef struct {
    /// The tag of the event.
    /// This should be CLIENT_DOOR_OPEN_EVENT_TAG or SERVER_DOOR_OPEN_EVENT_TAG
    uint16_t tag;
    /// The id of the door/lift that was opened.
    uint64_t door_id;
    /// Whether the door is a lift or not.
    bool is_lift;
} door_open_event_t;

/// door_close_event_t represents a door close event or lift descending event.
typedef struct {
    /// The tag of the event.
    /// This should be CLIENT_DOOR_CLOSE_EVENT_TAG or SERVER_DOOR_CLOSE_EVENT_TAG
    uint16_t tag;
    /// The id of the door/lift that was closed.
    uint64_t door_id;
    /// Whether the door is a lift or not.
    bool is_lift;
} door_close_event_t;

/// ClientDoorOpenEvent_new creates a new door_open_event_t.
inline door_open_event_t* ClientDoorOpenEvent_new(uint64_t door_id, bool is_lift) {
    door_open_event_t *event = (door_open_event_t*)malloc(sizeof(door_open_event_t));
    event->tag = CLIENT_DOOR_OPEN_EVENT_TAG;
    event->door_id = door_id;
    event->is_lift = is_lift;
    return event;
}

/// ClientDoorCloseEvent_new creates a new door_close_event_t.
inline door_close_event_t* ClientDoorCloseEvent_new(uint64_t door_id, bool is_lift) {
    door_close_event_t *event = (door_close_event_t*)malloc(sizeof(door_close_event_t));
    event->tag = CLIENT_DOOR_CLOSE_EVENT_TAG;
    event->door_id = door_id;
    event->is_lift = is_lift;
    return event;
}

/// ServerDoorOpenEvent_new creates a new door_open_event_t.
inline door_open_event_t* ServerDoorOpenEvent_new(uint64_t door_id, bool is_lift) {
    door_open_event_t *event = (door_open_event_t*)malloc(sizeof(door_open_event_t));
    event->tag = SERVER_DOOR_OPEN_EVENT_TAG;
    event->door_id = door_id;
    event->is_lift = is_lift;
    return event;
}

/// ServerDoorCloseEvent_new creates a new door_close_event_t.
inline door_close_event_t* ServerDoorCloseEvent_new(uint64_t door_id, bool is_lift) {
    door_close_event_t *event = (door_close_event_t*)malloc(sizeof(door_close_event_t));
    event->tag = SERVER_DOOR_CLOSE_EVENT_TAG;
    event->door_id = door_id;
    event->is_lift = is_lift;
    return event;
}

/// @brief Generate the door states of every doors in the world
/// @param world the ECS world
/// @param doors mapping of door ids to door entities
/// @param doors_states the resulting door states of every doors
/// @param doors_count the number of doors in doors
/// @return 0 if successful, -1 otherwise
int door_states_generate(world_t* world, entity_t** doors, bool* doors_states, int doors_count);

/// @brief Generate the lift states of every lifts in the world
/// @param world the ECS world
/// @param lifts mapping of lift ids to lift entities
/// @param lifts_states the resulting lift states of every lifts
/// @param lifts_count the number of lifts in lifts
/// @return 0 if successful, -1 otherwise
int lift_states_generate(world_t* world, entity_t** lifts, bool* lifts_states, int lifts_count);

#endif