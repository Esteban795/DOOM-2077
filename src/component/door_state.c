#include <stdlib.h>
#include <string.h>

#include "../../include/ecs/component.h"
#include "../../include/component/door_state.h"

const int COMPONENT_TAG_DOOR_STATE = 6;

component_t* door_state_create(unsigned int door_id, bool is_open) {
    door_state_ct* door_state_component = malloc(sizeof(door_state_ct));
    door_state_component->tag = COMPONENT_TAG_DOOR_STATE;
    door_state_component->door_id = door_id;
    door_state_component->is_open = is_open;
    return (component_t*) door_state_component;
}

extern inline unsigned int door_state_get_id(component_t* c);
extern inline bool door_state_is_open(component_t* c);