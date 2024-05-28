#ifndef _LIB_DOOM_COMPONENT_LIFT_STATE_H
#define _LIB_DOOM_COMPONENT_LIFT_STATE_H

#include <stdlib.h>
#include <stdbool.h>
#include "../ecs/component.h"

extern const int COMPONENT_TAG_LIFT_STATE;

/*
* lift_state_ct is a component that represents the current state of a lift.
* It is also linked the lift entity (entity_id) to the actual lift struct (lift_id).
*
* Lift are either high or low.
* Lift cannot be added programmatically, they are created by the map loader. The order 
* in which the lifts are loaded MUST be the same on the client and server. The `lift_id`
* directly corresponds to the lift loading order.
*/
typedef struct {
    int tag;
    unsigned int lift_id;
    bool is_open;
} lift_state_ct;

// lift_state_create creates a new lift state component with the given lift_id and state.
component_t* lift_state_create(unsigned int lift_id, bool is_open);

// lift_state_get_id returns the lift_id of the given lift_state component.
inline unsigned int lift_state_get_id(component_t* c) {
    return ((lift_state_ct*)c->data)->lift_id;
}

// lift_state_is_open returns the state of the given lift_state component.
inline bool lift_state_is_open(component_t* c) {
    return ((lift_state_ct*)c->data)->is_open;
}
#endif