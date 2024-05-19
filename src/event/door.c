#include "../../include/event/door.h"

extern inline door_open_event_t* ClientDoorOpenEvent_new(uint64_t door_id, bool is_lift);
extern inline door_close_event_t* ClientDoorCloseEvent_new(uint64_t door_id, bool is_lift); 
extern inline door_open_event_t* ServerDoorOpenEvent_new(uint64_t door_id, bool is_lift);
extern inline door_close_event_t* ServerDoorCloseEvent_new(uint64_t door_id, bool is_lift);

int door_states_generate(world_t* world, entity_t** doors, bool* doors_states, int doors_count) {
    for (int i = 0; i < doors_count; i++) {
        door_state_ct* state = (door_state_ct*) world_get_component(world, doors[i], COMPONENT_TAG_DOOR_STATE);
        if (state == NULL) return -1;
        doors_states[i] = state->is_open;
    }
    return 0;
}

int lift_states_generate(world_t* world, entity_t** lifts, bool* lifts_states, int lifts_count) {
    for (int i = 0; i < lifts_count; i++) {
        lift_state_ct* state = (lift_state_ct*) world_get_component(world, lifts[i], COMPONENT_TAG_LIFT_STATE);
        if (state == NULL) return -1;
        lifts_states[i] = state->is_open;
    }
    return 0;
}