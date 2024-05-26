#include "../../include/component/subsector_id.h"

const int COMPONENT_TAG_SUBSECTOR_ID = 8;

component_t* subsector_id_create(int subsector_id) {
    subsector_id_ct* component = malloc(sizeof(subsector_id_ct));
    component->tag = COMPONENT_TAG_SUBSECTOR_ID;
    component->subsector_id = subsector_id;
    return (component_t*)component;
}

extern inline int subsector_id_get(subsector_id_ct* subsector_id);

extern inline int subsector_id_set(subsector_id_ct* subsector_id, int subsector_id_value);