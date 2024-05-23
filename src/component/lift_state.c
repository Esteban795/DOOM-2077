#include <stdlib.h>
#include <string.h>

#include "../../include/ecs/component.h"
#include "../../include/component/lift_state.h"

const int COMPONENT_TAG_LIFT_STATE = 7;

component_t* lift_state_create(unsigned int lift_id, bool is_open) {
    lift_state_ct* lift_state_component = malloc(sizeof(lift_state_ct));
    lift_state_component->tag = COMPONENT_TAG_LIFT_STATE;
    lift_state_component->lift_id = lift_id;
    lift_state_component->is_open = is_open;
    return (component_t*) lift_state_component;
}

extern inline unsigned int lift_state_get_id(component_t* c);
extern inline bool lift_state_is_open(component_t* c);