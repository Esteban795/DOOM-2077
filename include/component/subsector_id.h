#ifndef _LIB_DOOM_COMPONENT_SUBSECTOR_ID_H
#define _LIB_DOOM_COMPONENT_SUBSECTOR_ID_H

#include "../ecs/component.h"
#include <stdlib.h>

extern const int COMPONENT_TAG_SUBSECTOR_ID;

/*
subsector_id_ct is a component that represents the subsector id of where an
entity currently is. It has only a single field : subsector_id. It is essential
for the rendering of ennemies.
*/

typedef struct {
  int tag;
  int subsector_id;
} subsector_id_ct;

component_t *subsector_id_create(int subsector_id);

inline int subsector_id_get(subsector_id_ct *subsector_id) {
  return subsector_id->subsector_id;
}

// Change subsector_id to a new value
inline int subsector_id_set(subsector_id_ct *subsector_id,
                            int subsector_id_value) {
  subsector_id->subsector_id = subsector_id_value;
  return subsector_id->subsector_id;
}

#endif